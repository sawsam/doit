/*
 * framesendbase.cpp
 *
 *  Created on: Oct 18, 2010
 *      Author: work
 */

#include "FramesendBase.h"
#include "common/message/message_id.h"

using namespace sdk;

namespace common
{
FramesendBase::FramesendBase(const FramesendBaseConfig& config,CommModule* server)
:EngineBase(config._config),_server(server),_timeout(config._timeout),_bufsize(config._bufsize),_send_packet(0),_send_size(0),_print_timeval(config._print_timeval)
{
	insert_default_handle(&FramesendBase::default_call);
	insert_handle(MCI_FRAMESEND_UNIT_ADD,&FramesendBase::unit_add);
	insert_handle(MCI_FRAMESEND_UNIT_DEL,&FramesendBase::unit_del);
}

FramesendBase::~FramesendBase()
{
	for (UnitMap::iterator iter = _units.begin(); iter != _units.end(); ++iter)
	{
		if(iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	_units.clear();
}

bool FramesendBase::start()
{
	if (!EngineBase::start())
	{
		return false;
	}

	TimerHandlerPtr timer(new SendTimer(this));
	add_timer(_timeout,true,timer);

	if (_print_timeval)
	{
		TimerHandlerPtr print_timer(new PrintTimer(this));
		add_timer(_print_timeval,true,print_timer);
	}

	return true;
}

bool FramesendBase::stop()
{
	EngineBase::stop();

	return true;
}

bool FramesendBase::add_fd_sn(int fd,int32 sn)
{
	ServerPacket* p = new ServerPacket(fd,sn,NULL,0);
	if (!p)
	{
		return false;
	}
	p->set_code(MCI_FRAMESEND_UNIT_ADD);
	push_packet(p);

	return true;
}

bool FramesendBase::del_fd_sn(int fd,int32 sn)
{
	ServerPacket* p = new ServerPacket(fd,sn,NULL,0);
	if (!p)
	{
		return false;
	}
	p->set_code(MCI_FRAMESEND_UNIT_DEL);
	push_packet(p);

	return true;
}

PacketHandlerResult FramesendBase::unit_add(ServerPacket* packet)
{
	uint64 key = MAKEINT64(packet->get_fd(),packet->get_sn());
	UnitMap::iterator iter = _units.find(key);
	if (iter != _units.end())
	{
		_units.erase(iter->first);
		delete iter->second;
	}

	FrameSendUnit* unit = new FrameSendUnit(packet->get_fd(),packet->get_sn(),_bufsize);
	if (!unit)
	{
		return DELETE_PACKET;
	}
	if (!unit->_buf)
	{
		delete unit;
		return DELETE_PACKET;
	}

	_units[key] = unit;

	return DELETE_PACKET;
}

PacketHandlerResult FramesendBase::unit_del(ServerPacket* packet)
{
	uint64 key = MAKEINT64(packet->get_fd(),packet->get_sn());
	UnitMap::iterator iter = _units.find(key);
	if (iter != _units.end())
	{
		delete iter->second;
		_units.erase(key);
	}

	return DELETE_PACKET;
}

//--如果缓冲区满就直接发到队列里面(也就是底层的 _send_q 队列)，否则就直接拷贝到缓冲区
//--也就是每个player的缓冲区里，就是每个fd的缓冲区，这个时候包头还没有写内容
void FramesendBase::frame_send(FrameSendUnit* unit,PacketBase* packet)
{
	uint16 msg_len = (uint16)packet->get_msg_size();	//这里要注意，有可能会发生截断的
	const uint16 frame_len = (msg_len + (uint16)sizeof(uint16));

	//--缓冲区已经装不下了，先把缓冲区内容发送出去
	if (frame_len > (_bufsize - unit->_size))
	{
		send_buffer(unit);
	}

	//--如果这个包比缓冲区太大就直接发送到消息队列
	if(frame_len > _bufsize)
	{
		if (frame_len > MAX_FRAMESEND_MSG_SIZE)
		{
			return;
		}
		char tempbuf[MAX_FRAMESEND_MSG_SIZE];
		memcpy(&tempbuf[0], &msg_len, sizeof(uint16));
		memcpy(&tempbuf[sizeof(uint16)], packet->get_msg_buffer(), msg_len);

		_server->request_send_message(unit->_fd,unit->_sn,&tempbuf[0],frame_len);
	}
	else
	{
		//--暫时先放到缓冲区
		memcpy(&unit->_buf[unit->_size], &msg_len, sizeof(uint16));
		unit->_size += sizeof(uint16);

		memcpy(&unit->_buf[unit->_size], packet->get_msg_buffer(), msg_len);
		unit->_size += msg_len;
	}
}

void FramesendBase::send_buffer(FrameSendUnit * unit)
{
	_server->request_send_message(unit->_fd,unit->_sn,unit->_buf,unit->_size);

	//--缓存满了，只是简单的将size置0,并没有擦除内容
	unit->_size = 0;
}
}
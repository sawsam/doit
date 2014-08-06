#ifndef CLIENTNET_H_
#define CLIENTNET_H_

#include "stdafx.h"

#include "communicate/CommModule.h"
#include "common/framesend/ServerFramesend.h"
#include "tools/ClientPacket.h"
#include "common/message/message_id.h"
#include "StreamRW.h"
#include "common/shared/datatype_define.h"

using namespace sdk;
using namespace common;
using namespace std;

class ClientNet : public CommInterface
{
public:
	static ClientNet* instance()
	{
		static ClientNet net;
		return &net;
	}

	void enable_acceptor()
	{
		_com_config.enable_acceptor();
	}

	void set_listen_port(int port)
	{
		_com_config.set_listen_port(port);
	}

	void set_max_read_unit(int mru)
	{
		_com_config.set_max_read_unit(mru);
	}

	bool start()
	{
		if (_working)
		{
			return true;
		}
		_working = true;

		_comm = new CommModule(_com_config,this);
		if (!_comm->start())
		{
			stop();
			return false;
		}

		return true;
	}

	bool stop()
	{
		if (!_working)
		{
			return true;
		}

		_working = false;
		if (_comm)
		{
			_comm->stop();
			delete _comm;
			_comm = NULL;
		}

		return true;
	}

	void packet_reset()
	{
		_mq.swap();
	}

	bool pick(int& fd,int& sn,int64& uid,int& pid,int& code,int& err)
	{
		queue<ServerPacket*>* rq = _mq.get_rq();
		if (!rq->empty())
		{
			ServerPacket* packet = rq->front();
			rq->pop();
			fd = packet->get_fd();
			sn = packet->get_sn();
			uid = packet->get_uid();
			pid = packet->get_pid();
			code = packet->get_code();
			err = packet->get_error();
			StreamRW::instance()->set_read_packet(packet);
			return true;
		}
		return false;
	}

	virtual void notice_connection_open(int fd,int sn)
	{
		ServerPacket* packet = new ServerPacket();
		packet->set_code(MCI_CONNECTION_OPEN);
		packet->set_fd(fd);
		packet->set_sn(sn);
		push_packet(packet);
	}

	virtual void notice_connection_close(int fd,int sn)
	{
		ServerPacket* packet = new ServerPacket();
		packet->set_code(MCI_CONNECTION_CLOSE);
		packet->set_fd(fd);
		packet->set_sn(sn);
		push_packet(packet);
	}

	virtual void notice_message_arrive(int fd,int sn,char* msg,size_t len)
	{
		uint16 offset = 0;
		while(true)
		{
			if(len <= offset)
			{
				break;
			}

			uint16 msg_len(0);
			memcpy(&msg_len, (msg + offset), sizeof(uint16));

			offset += sizeof(uint16);

			if(msg_len > (len - offset))
			{
				return;
			}

			ClientPacket p(fd,sn,msg+offset,msg_len);
			ServerPacket* c = new ServerPacket(p);
			if (!c)
			{
				return;
			}
			push_packet(c);
			offset += msg_len;
		}	
	}

	virtual void notice_connect_error(int sn)
	{
		ServerPacket* packet = new ServerPacket();
		packet->set_code(MCI_CONNECT_ERROR);
		packet->set_sn(sn);
		push_packet(packet);
	}

	virtual void request_connect_server(const ConnectInfo& ci)
	{
		if (_comm)
		{
			_comm->request_connect_server(ci);
		}
	}

	virtual void request_close_connection(int fd,int sn)
	{
		if (_comm)
		{
			_comm->request_close_connection(fd,sn);
		}
	}

	void push_packet(ServerPacket* packet)
	{
		_mq.push(packet);
	}

	void send_packet(ServerPacket* packet)
	{
		ClientPacket c(*packet);
		if (_comm)
		{
			_comm->request_send_message(c.get_fd(),c.get_sn(),c.get_msg_buffer(),c.get_msg_size());
		}
		delete packet;
	}

	void send_packet()
	{
		ServerPacket* packet = StreamRW::instance()->get_write_packet();
		if (packet)
		{
			send_packet(packet);
			StreamRW::instance()->reset_write_packet();
		}
	}

private:
	ClientNet():_comm(NULL),_working(false) {}
	~ClientNet() {}

private:
	CommConfig _com_config;
	CommModule* _comm;
	bool _working;
	rw_queue<ServerPacket*> _mq;
};

#endif /* CLIENTNET_H_ */
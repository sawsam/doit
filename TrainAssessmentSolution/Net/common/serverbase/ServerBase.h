#ifndef SERVERBASE_H_
#define SERVERBASE_H_

#include "enginebase/EngineBase.h"
#include "communicate/CommModule.h"
#include "ServerBaseConfig.h"
#include "common/framesend/ClientFramesend.h"
#include "common/message/message_id.h"
#include "tools/ClientPacket.h"

using namespace sdk;

namespace common
{
class ServerBase : public EngineBase, public CommInterface
{
public:
	ServerBase(const ServerBaseConfig& config)
	:EngineBase(config._engineconfig),_config(config),_comm(NULL),_frame_send(NULL)
	{
	}
	virtual ~ServerBase()
	{
	}

	/*
		����������������
		���أ�true�����ɹ���false����ʧ��
	*/
	bool start()
	{
		if (_ef_working)
		{
			return true;
		}

		_comm = new CommModule(_config._commconfig,this);
		if (!_comm->start())
		{
			stop();
			return false;
		}

		_frame_send = new ClientFramesend(_config._framesendconfig,_comm);
		if (!_frame_send->start())
		{
			stop();
			return false;
		}

		if (!EngineBase::start())
		{
			stop();
			return false;
		}

		return true;
	}

	/*
		�������رշ�����
		���أ�true�رճɹ���false�ر�ʧ��
	*/
	bool stop()
	{
		EngineBase::stop();
		if (_frame_send)
		{
			_frame_send->stop();
			delete _frame_send;
			_frame_send = NULL;
		}
		if (_comm)
		{
			_comm->stop();
			delete _comm;
			_comm = NULL;
		}

		return true;
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

	/*
		��������Ϣ���ﴦ�������ְ�
	*/
	virtual void notice_message_arrive(int fd,int sn,char* msg,size_t len)
	{
		ClientPacket c(fd, sn, msg, len);
		ServerPacket *s = new ServerPacket(c);
		if(!s)
		{
			return;
		}

		push_packet(s);
	}

	virtual void notice_connect_error(int sn)
	{

	}

	void send_packet(ServerPacket* packet)
	{
		if (!_frame_send)
		{
			delete packet;
			return;
		}
		_frame_send->push_packet(packet);
	}

	void send_packet(int fd,int sn,uint64 uid,int32 pid,uint16 code,uint16 err)
	{
		if (!_frame_send)
		{
			return;
		}
		ServerPacket* packet = new ServerPacket(fd,sn,uid,pid,code,err);
		_frame_send->push_packet(packet);
	}

protected:
	ServerBaseConfig _config;//server����
	CommModule* _comm;//����ģ��
	ClientFramesend* _frame_send;//��֡����ģ��
};
}

#endif /* SERVERBASE_H_ */
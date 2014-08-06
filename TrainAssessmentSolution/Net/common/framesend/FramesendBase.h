/*
 * framesendbase.h
 *
 *  Created on: Oct 18, 2010
 *      Author: work
 */

#ifndef FRAMESENDBASE_H_
#define FRAMESENDBASE_H_

#include "enginebase/EngineBase.h"
#include "FrameSendBaseConfig.h"
#include "communicate/CommModule.h"
#include "tools/ServerPacket.h"

using namespace sdk;

namespace common
{
#define MAX_FRAMESEND_MSG_SIZE 65536
class FramesendBase : public EngineBase
{
public:
	FramesendBase(const FramesendBaseConfig& config,CommModule* server);
	virtual ~FramesendBase();

	bool start();

	bool stop();

	bool add_fd_sn(int fd,int32 sn);

	bool del_fd_sn(int fd,int32 sn);

protected:

	//--这个FrameSendUnit相当于每个连接到服务器的套接字
	struct FrameSendUnit
	{
		FrameSendUnit(int fd,int32 sn,uint16 bufsize)
		:_fd(fd),_sn(sn),_buf(new char[bufsize]),_size(0)
		{
			
		}

		~FrameSendUnit()
		{
			if (_buf)
			{
				delete [] _buf;
			}
		}

		int _fd;
		int32 _sn;
		char* _buf;
		uint16 _size;
	};

	class SendTimer : public TimerHandler
	{
	public:
		SendTimer(FramesendBase * frame_send)
		:_frame_send(frame_send)
		{
			
		}
		virtual ~SendTimer(){}

		//--这里才是定时把数据包发送到发送队列里面(每10毫秒一次操作)
		virtual TimerHandleResult on_timer()
		{
			for (UnitMap::iterator iter = _frame_send->_units.begin(); iter != _frame_send->_units.end(); ++iter)
			{
				if (iter->second->_size > 0)
				{
					if (_frame_send->_print_timeval)
					{
						_frame_send->_send_packet ++;
						_frame_send->_send_size += iter->second->_size;
					}

					_frame_send->send_buffer(iter->second);
				}
			}

			return THR_NORMAL;
		}

		FramesendBase * _frame_send;
	};

	class PrintTimer : public TimerHandler
	{
	public:
		PrintTimer(FramesendBase* fs)
		:_fs(fs)
		{
			
		}
		virtual ~PrintTimer()
		{

		}

		virtual TimerHandleResult on_timer()
		{ 
			static int32 count = 0;
			int64 standy_num = _fs->get_standby_num();
			if (standy_num > 100000)
			{
				ofstream f;
				f.open("pressure_test.log",ios::out | ios::app);
				if (f.is_open())
				{
					boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
					string s = boost::posix_time::to_simple_string(t);
					stringstream ss;
					ss << "[" << s << "]" << "[Framesend] standby num: " << standy_num << endl;
					ss << "[" << s << "]" << "[Framesend] send: " << _fs->_send_packet << " size: " << _fs->_send_size << endl;
					f << ss.str();
					f.close();
				}
			}
			if (++count == 10)
			{
				cout << "[Framesend] standby num: " << standy_num << endl;
				cout << "[Framesend] send: " << _fs->_send_packet << " size: " << _fs->_send_size << endl;
				count = 0;
			}
			_fs->_send_packet = _fs->_send_size = 0;
			return THR_NORMAL;
		}

		FramesendBase* _fs;
	};


protected:
	typedef hash_map<uint64,FrameSendUnit*> UnitMap;
	UnitMap _units;
	CommModule* _server;
	int32 _timeout;
	uint16 _bufsize;
	uint32 _send_packet;
	uint32 _send_size;
	int32 _print_timeval;

protected:
	virtual PacketHandlerResult default_call(ServerPacket* packet) = 0;

	PacketHandlerResult unit_add(ServerPacket* packet);

	PacketHandlerResult unit_del(ServerPacket* packet);

	void frame_send(FrameSendUnit* unit,PacketBase* packet);

	void send_buffer(FrameSendUnit * unit);
};
}

#endif /* FRAMESENDBASE_H_ */

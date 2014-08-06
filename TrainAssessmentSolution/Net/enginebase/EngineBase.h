#ifndef ENGINEBASE_H_
#define ENGINEBASE_H_

#include "eventframe/EventFrame.h"
#include "util/rw_queue.h"
#include "util/SimpleTimer.h"
#include "tools/ServerPacket.h"
#include "tools/PacketHandler.h"
#include "TimerHandler.h"
#include "EngineBaseConfig.h"

namespace sdk
{
class EngineBase : public PacketHandler, public EventFrame, public PacketHandleHashTable
{
	typedef priority_queue<TimerNode*, vector<TimerNode*> , TimerNodeCompare> TIMER_QUEUE;
	typedef hash_map<int32,TimerNode*> TIMER_MAP;
public:
	EngineBase(const EngineBaseConfig& config)
	:_config(config),_mq(),_cq(),_now_time(0),_max_timer_id(0),_standby_num(0),_idle_timer(_io_service)
	{
	}

	virtual ~EngineBase()
	{
		{
			_mq.swap();
			queue<ServerPacket*>* rq = _mq.get_rq();
			queue_clean_and_free(*rq);
		}

		{
			_cq.swap();
			queue<TimerCommand*>* rq = _cq.get_rq();
			queue_clean_and_free(*rq);
		}

		hash_map_clean_and_free(_timer_map);
	}

	/*
		����������Ϣ��������Ϣ����
		������packet�������Ϣ��
	*/
	inline void push_packet(ServerPacket* packet)
	{
		boost::interprocess::ipcdetail::atomic_inc32(&_standby_num);
		_mq.push(packet);
	}

	/*
		���������timer
		������timeout��ʱʱ��ms��reload��ʱ���Ƿ��������ã�handler��ʱ�ص�
		���أ�timer id
	*/
	int32 add_timer(int32 timeout/*ms*/, bool reload, TimerHandlerPtr & handler)
	{
		int32 timer_id = ++_max_timer_id;
		TimerCommand* c = new TimerCommand(timer_id,timeout,reload,handler);
		_cq.push(c);
		return timer_id;
	}

	/*
		������ɾ��timer
		������timer id
	*/
	void del_timer(int32 timer_id)
	{
		TimerCommand* c = new TimerCommand(timer_id);
		_cq.push(c);
	}

	virtual PacketHandleTable * get_handle_table()
	{
		return this;
	}

	inline uint32 get_standby_num()
	{
		return boost::interprocess::ipcdetail::atomic_read32(&_standby_num);
	}

private:
	virtual void user_process()
	{
		mq_process();
		cq_process();
		timer_process();
	}

	/*
		��������Ϣ���д�����
	*/
	void mq_process()
	{
		_mq.swap();
		queue<ServerPacket*>* rq = _mq.get_rq();
		size_t size = rq->size();
		for (size_t i = 0; i < size; ++i)
		{
			ServerPacket* packet = rq->front();
			rq->pop();
			if (_config._slow_log)
			{
				_t.start();
			}
			PacketHandlerResult result = dispatch_packet(packet);
			if (_config._slow_log)
			{
				_t.stop();
				int64 timeuse = _t.interval();
				if (timeuse > _config._slow_log_limit)
				{
					write_slow_log(packet->get_code(),timeuse);
				}
			}
			if ( result!= REUSE_PACKET)
			{
				delete packet;
			}
			boost::interprocess::ipcdetail::atomic_dec32(&_standby_num);
		}

		if (size == 0)
		{//��Ϣ����Ϊ����ȴ�
			_idle_timer.expires_from_now(boost::posix_time::microseconds(_config._idle_wait));
			_idle_timer.wait();
		}
	}

	/*
		������������д�����
	*/
	void cq_process()
	{
		_cq.swap();
		queue<TimerCommand*>* rq = _cq.get_rq();
		size_t size = rq->size();
		for (size_t i = 0; i < size; ++i)
		{
			TimerCommand* c = rq->front();
			rq->pop();
			switch(c->_type)
			{
			case TCT_ADD:
				timer_add(c);
				break;
			case TCT_DEL:
				timer_del(c);
				break;
			default:
				break;
			}
			delete c;
		}
	}

	/*
		������timer������
	*/
	void timer_process()
	{
		int64 time_period = _time_ruler.measure();
		if (time_period < 0)
		{
			time_period = 0;
		}
		_now_time += time_period;

		while(!_timer_q.empty())
		{
			TimerNode * t = _timer_q.top();
			if(t->_time_dest <= _now_time)
			{
				_timer_q.pop();
				if(t->_cancelled == false)
				{
					t->_counter++;

					switch(t->_handler->on_timer())
					{
					case THR_RELOAD:
						t->_reload = true;
						break;
					case THR_DEL:
						t->_reload = false;
						break;
					default:
						break;
					}

					if(t->_reload)
					{
						t->_time_dest = _now_time + t->_interval;
						_timer_q.push(t);
					}
					else
					{
						_timer_map.erase(t->_timer_id);
						delete t;
					}
				}
				else
				{
					_timer_map.erase(t->_timer_id);
					delete t;
				}
			}
			else
			{
				break;
			}
		}
	}

	void timer_add(TimerCommand* c)
	{
		TimerNode* n = new TimerNode(_now_time + c->_interval,c->_interval,c->_timer_id,c->_reload,c->_handler);
		_timer_q.push(n);
		_timer_map.insert(make_pair(c->_timer_id,n));
	}

	void timer_del(TimerCommand* c)
	{
		TIMER_MAP::iterator iter = _timer_map.find(c->_timer_id);
		if (iter == _timer_map.end())
		{
			return;
		}
		iter->second->_cancelled = true;
	}

	void write_slow_log(uint16 code,int64 timeuse)
	{
		boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
		string s = boost::posix_time::to_simple_string(t);
		stringstream ss;
		ss << "[" << s << "] msg: " << code << " timeuse: " << timeuse << "us" << endl;
		ofstream f;
		f.open(_config._slow_log_filename,ios::out | ios::app);
		if (!f.is_open())
		{
			return;
		}
		f << ss.str();
		f.close();
	}

private:
	EngineBaseConfig _config;//engine����
	rw_queue<ServerPacket*> _mq;//��Ϣ����
	rw_queue<TimerCommand*> _cq;//�������
	TIMER_QUEUE _timer_q;//timer���ȶ���
	TIMER_MAP _timer_map;//timer��ϣ�б�
	int64 _now_time;//��ǰʱ�䣨��Գ�������
	SimpleTimeRuler _time_ruler;//ʱ�����
	int32 _max_timer_id;//timer id
	uint32 _standby_num;//���������Ϣ��
	boost::asio::io_service _io_service;//io_service
	boost::asio::deadline_timer _idle_timer;//���еȴ�timer
	SimpleTimer _t;//����Ϣͳ��
};
}

#endif /* ENGINEBASE_H_ */
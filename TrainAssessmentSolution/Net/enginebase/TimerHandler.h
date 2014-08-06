#ifndef TIMERHANDLER_H_
#define TIMERHANDLER_H_

#include "global/base_inc.h"

namespace sdk
{
//timer处理返回值
enum TimerHandleResult
{
	THR_NORMAL = 0,//正常
	THR_RELOAD = 1,//重新设置
	THR_DEL = 2,//删除
};

class TimerHandler
{
public:
	TimerHandler() {}
	virtual ~TimerHandler() {}

	virtual TimerHandleResult on_timer() = 0;
};
typedef boost::shared_ptr<TimerHandler> TimerHandlerPtr;

//timer节点
struct TimerNode
{
	TimerNode(int64 time_dest/*ms*/, int64 interval/*ms*/, int32 timer_id, bool reload, TimerHandlerPtr & handler) :
		_time_dest(time_dest), _interval(interval), _counter(0), _timer_id(timer_id)
	, _reload(reload), _cancelled(false), _handler(handler)
	{}
	~TimerNode()
	{}

	int64 _time_dest;
	int64 _interval;
	int64 _counter;
	int32 _timer_id;
	bool _reload;
	bool _cancelled;
	TimerHandlerPtr _handler;
};

struct TimerNodeCompare
{
	bool operator()(TimerNode * p1, TimerNode * p2)
	{
		return p1->_time_dest > p2->_time_dest;
	}
};

//timer命令类型
enum TimerCommandType
{
	TCT_ADD = 0,
	TCT_DEL = 1,
};

//timer命令
struct TimerCommand
{
	TimerCommand(int32 timer_id,int32 interval,bool reload,TimerHandlerPtr handler)
	:_type(TCT_ADD),_timer_id(timer_id),_interval(interval),_reload(reload),_handler(handler)
	{
	}

	TimerCommand(int32 timer_id)
	:_type(TCT_DEL),_timer_id(timer_id),_interval(0),_reload(false),_handler()
	{
	}
	
	TimerCommandType _type;
	int32 _timer_id;
	int32 _interval;
	bool _reload;
	TimerHandlerPtr _handler;
};
}

#endif /* TIMERHANDLER_H_ */
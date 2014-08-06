/********************************************************************
	created:	2013-4-19   09:48
	filename: 	GlobalTime.h
	file path:	...\sdk\common\shared
	author:		hc
	
	purpose:	获取时间，定时器及输出错误信息
*********************************************************************/

#ifndef GLOBALTIME_H_
#define GLOBALTIME_H_

#include "global/base_inc.h"
#include "gametime.h"

namespace common
{

/** 定时器类型 */
enum SpecifyTimerType
{
	STT_NULL = 0,
	/** 第二天 */
	STT_NEXT_DAY = 1,
};

/** 定时器 */
struct SpecifyTimer
{
	time_t _time;
	SpecifyTimerType _type;
	int32 _param1;
	int32 _param2;

	SpecifyTimer():_time(0), _type(STT_NULL), _param1(0), _param2(0)
	{

	}

	SpecifyTimer(time_t time, SpecifyTimerType type, int32 param1 = 0, int32 param2 = 0):_time(time), _type(type), _param1(param1), _param2(param2)
	{

	}

	SpecifyTimer(const SpecifyTimer &copy):_time(copy._time), _type(copy._type), _param1(copy._param1), _param2(copy._param2)
	{

	}

	virtual ~SpecifyTimer()
	{

	}
};

struct SpecifyTimerLess
{
	inline bool operator () (const SpecifyTimer *__x, const SpecifyTimer *__y) const
	{
		return __x->_time > __y->_time;
	}
};


class GlobalTime
{
private:
	/** 当前时间 */
	time_t _now;

	/** 定时器列表 */
	priority_queue<SpecifyTimer *, vector<SpecifyTimer *>, SpecifyTimerLess> _timer_queue;

	/** 开服时间 */
	time_t _open_time;

private:
	GlobalTime();

	virtual ~GlobalTime();

public:
	inline static GlobalTime *instance()
	{
		static GlobalTime timer;
		return &timer;
	}

	/** 获取当前时间，只在主逻辑使用 */
	inline time_t get_now_sec()
	{
		return _now;
	}

	/** 更新当前时间，只在主逻辑使用 */
	inline void update_now()
	{
		_now = time(NULL);
	}

	/** 获取开服时间（秒） */
	inline time_t get_server_open_time()
	{
		return _open_time;
	}

	/** 设置开服时间 */
	inline void set_server_open_time(time_t time)
	{
		if (time != 0)
		{
			_open_time = time;
		}
	}

	/** 获取开服后的天数 */
	int32 get_server_day_from_open();

	/** 添加时间点到定时列表，并保持唯一性，只在需要定时处理的主逻辑使用或主逻辑运行之前使用 */
	void timer_queue_add(const SpecifyTimer &timer);

	/** 只在需要定时处理的主逻辑使用或主逻辑运行之前使用 */
	bool timer_queue_empty();

	/** 只在需要定时处理的主逻辑使用或主逻辑运行之前使用 */
	const SpecifyTimer & timer_queue_top();

	/** 只在需要定时处理的主逻辑使用或主逻辑运行之前使用 */
	void timer_queue_pop();

	string log_assert_string(const string &x, const string &file, int line, const string &func);

	void log_assert(const string &x, const string &file, int line, const string &func);
	
	void log_to_db(const string &content);
};

//记录到logserver
#define LOG_ASSERT(x) if (!(x)) { GlobalTime::instance()->log_assert(#x, __FILE__, __LINE__, __FUNCTION__); }

#define LOG_ASSERT_EX(x, num) if (!(x)) { string s = #x + to_string(" num: ") + to_string(num); GlobalTime::instance()->log_assert(s, __FILE__, __LINE__, __FUNCTION__); }

}


#endif // GLOBALTIME_H_
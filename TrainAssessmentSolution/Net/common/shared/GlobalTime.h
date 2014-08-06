/********************************************************************
	created:	2013-4-19   09:48
	filename: 	GlobalTime.h
	file path:	...\sdk\common\shared
	author:		hc
	
	purpose:	��ȡʱ�䣬��ʱ�������������Ϣ
*********************************************************************/

#ifndef GLOBALTIME_H_
#define GLOBALTIME_H_

#include "global/base_inc.h"
#include "gametime.h"

namespace common
{

/** ��ʱ������ */
enum SpecifyTimerType
{
	STT_NULL = 0,
	/** �ڶ��� */
	STT_NEXT_DAY = 1,
};

/** ��ʱ�� */
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
	/** ��ǰʱ�� */
	time_t _now;

	/** ��ʱ���б� */
	priority_queue<SpecifyTimer *, vector<SpecifyTimer *>, SpecifyTimerLess> _timer_queue;

	/** ����ʱ�� */
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

	/** ��ȡ��ǰʱ�䣬ֻ�����߼�ʹ�� */
	inline time_t get_now_sec()
	{
		return _now;
	}

	/** ���µ�ǰʱ�䣬ֻ�����߼�ʹ�� */
	inline void update_now()
	{
		_now = time(NULL);
	}

	/** ��ȡ����ʱ�䣨�룩 */
	inline time_t get_server_open_time()
	{
		return _open_time;
	}

	/** ���ÿ���ʱ�� */
	inline void set_server_open_time(time_t time)
	{
		if (time != 0)
		{
			_open_time = time;
		}
	}

	/** ��ȡ����������� */
	int32 get_server_day_from_open();

	/** ���ʱ��㵽��ʱ�б�������Ψһ�ԣ�ֻ����Ҫ��ʱ��������߼�ʹ�û����߼�����֮ǰʹ�� */
	void timer_queue_add(const SpecifyTimer &timer);

	/** ֻ����Ҫ��ʱ��������߼�ʹ�û����߼�����֮ǰʹ�� */
	bool timer_queue_empty();

	/** ֻ����Ҫ��ʱ��������߼�ʹ�û����߼�����֮ǰʹ�� */
	const SpecifyTimer & timer_queue_top();

	/** ֻ����Ҫ��ʱ��������߼�ʹ�û����߼�����֮ǰʹ�� */
	void timer_queue_pop();

	string log_assert_string(const string &x, const string &file, int line, const string &func);

	void log_assert(const string &x, const string &file, int line, const string &func);
	
	void log_to_db(const string &content);
};

//��¼��logserver
#define LOG_ASSERT(x) if (!(x)) { GlobalTime::instance()->log_assert(#x, __FILE__, __LINE__, __FUNCTION__); }

#define LOG_ASSERT_EX(x, num) if (!(x)) { string s = #x + to_string(" num: ") + to_string(num); GlobalTime::instance()->log_assert(s, __FILE__, __LINE__, __FUNCTION__); }

}


#endif // GLOBALTIME_H_
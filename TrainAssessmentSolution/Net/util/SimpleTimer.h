#ifndef SIMPLETIMER_H_
#define SIMPLETIMER_H_

#include "global/base_inc.h"

namespace sdk
{
struct TimeInfo
{
	int16 _year;
	int8 _month;
	int8 _day;

	int8 _hour;
	int8 _min;
	int8 _sec;

	int8 _weekday;		//星期几 [0-6], 0是星期天

	int32 _total_sec;	//秒
	int64 _total_usec;	//微秒

	string _format;

	TimeInfo()
	{
		_year = 0;
		_month = 0;
		_day = 0;

		_hour = 0;
		_min = 0;
		_sec = 0;

		_weekday = -1;

		_total_sec = 0;
		_total_usec = 0;

		_format = "";
	}

	~TimeInfo()
	{

	}
};

class SimpleTimer
{
public:
	SimpleTimer() {}
	~SimpleTimer() {}

	inline void start()
	{
		//-- 2013-Apr-27 09:27:26.129666
		_start = boost::posix_time::microsec_clock::local_time();
	}

	inline void stop()
	{
		_stop = boost::posix_time::microsec_clock::local_time();
	}

	inline int64 interval()
	{
		return (_stop - _start).total_microseconds();
	}

	static void now_info(TimeInfo & time_info)
	{

		boost::posix_time::ptime epoch_time(boost::gregorian::date(1970,1,1));
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = now - epoch_time;
		std::tm now_tm = boost::posix_time::to_tm(now);

		time_info._year = now_tm.tm_year + 1900;
		time_info._month = now_tm.tm_mon + 1;
		time_info._day = now_tm.tm_mday;

		time_info._hour = now_tm.tm_hour;
		time_info._min = now_tm.tm_min;
		time_info._sec =  now_tm.tm_sec;

		time_info._weekday =  now_tm.tm_wday;

		time_info._total_sec = diff.total_seconds();
		time_info._total_usec = diff.total_microseconds();

		char temp_format[100];
		_snprintf_s(temp_format, 100, "%d-%02d-%02d %02d:%02d:%02d", time_info._year, time_info._month, time_info._day, time_info._hour, time_info._min, time_info._sec);

		time_info._format = temp_format;
	}

private:
	boost::posix_time::ptime _start;
	boost::posix_time::ptime _stop;
};

class SimpleTimeRuler
{
public:
	SimpleTimeRuler():_last_time_tick( boost::posix_time::microsec_clock::local_time()) {}
	~SimpleTimeRuler() {}

	inline int64 measure()
	{
		boost::posix_time::ptime now_time_tick(boost::posix_time::microsec_clock::local_time());
		int64 time_period = (now_time_tick - _last_time_tick).total_milliseconds();
		_last_time_tick = now_time_tick;
		return time_period;
	}
private:
	boost::posix_time::ptime _last_time_tick;
};
}

#endif /* SIMPLETIMER_H_ */
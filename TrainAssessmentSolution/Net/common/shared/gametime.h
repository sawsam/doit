/********************************************************************
	created:	2013-4-19   10:50
	filename: 	gametime.h
	file path:	...\sdk\common\shared
	author:		work
	
	purpose:	
*********************************************************************/

#ifndef GAMETIME_H_
#define GAMETIME_H_

#include "global/base_inc.h"

#ifdef WIN32
#include <time.h>
#define LOCALTIME_SAFE(t, tm) localtime_s((tm), (t))	//win32����linux����������λ���෴
#else
#include <sys/time.h>
#define LOCALTIME_SAFE(t, tm) localtime_r((t), (tm))
#endif

struct DayTime
{
	int32 _hour;
	int32 _min;
	int32 _sec;

	DayTime()
	{
		_hour = 0;
		_min = 0;
		_sec = 0;
	}

	DayTime(int32 hour,int32 min,int32 sec)
	{
		_hour = hour;
		_min = min;
		_sec = sec;
	}

	~DayTime()
	{
		_hour = 0;
		_min = 0;
		_sec = 0;
	}

	int32 day_pass()
	{
		int32 seconds = _hour * 3600 + _min * 60 + _sec;

		return seconds;
	}
};

/** ���ǻ�ȡ2000-01-01 00:00:00��ʱ�䣨�룩������ϵͳʱ���仯 */
static time_t get_sec_year_2000()
{
	static time_t sec = 0;
	if (sec == 0)
	{
		struct tm date = { 0 };
		date.tm_year = 100;
		date.tm_mon = 0;
		date.tm_mday = 1;
		date.tm_hour = 0;
		date.tm_min = 0;
		date.tm_sec = 0;
		sec = mktime(&date);
	}
	return sec;
}

/** ���ǻ�ȡ2000��һ������һ 00:00:00��ʱ�䣨�룩������ϵͳʱ���仯 */
static time_t get_sec_first_monday_of_year_2000()
{
	static time_t sec = 0;
	if (sec == 0)
	{
		//2000-01-03��������һ
		struct tm date = { 0 };
		date.tm_year = 100;
		date.tm_mon = 0;
		date.tm_mday = 3;
		date.tm_hour = 0;
		date.tm_min = 0;
		date.tm_sec = 0;
		sec = mktime(&date);
	}
	return sec;
}

/** ���ǻ�ȡ2000-01-01 00:00:00��������������������ϵͳʱ���仯 */
inline static int32 get_day_from_2000(time_t sec)
{
	const int32 sec_of_day = 3600 * 24;
	return (int32)(sec - get_sec_year_2000()) / sec_of_day;
}

/** ���ǻ�ȡ2000-01-01 00:00:00����������Сʱ��������ϵͳʱ���仯 */
inline static int32 get_hour_from_2000(time_t sec)
{
	const int32 sec_of_hour = 3600;
	return (int32)(sec - get_sec_year_2000()) / sec_of_hour;
}

/** ���ǻ�ȡ2000-01-01 00:00:00��������������������ϵͳʱ���仯 */
inline static int32 get_second_from_2000(time_t sec)
{
	return (int32)(sec - get_sec_year_2000());
}

/** ���ǻ�ȡ2000-01-01 04:00:00��������������������ϵͳʱ���仯 */
inline static int32 get_refresh_day_from_2000(time_t sec)
{
	const int32 sec_of_day = 3600 * 24;
	return (int32)(sec - get_sec_year_2000() - (3600 * 4)) / sec_of_day;
}

/** ���ǻ�ȡ2000-01-01 00:30:00��������������������ϵͳʱ���仯 */
inline static int32 get_refresh_day_ex_from_2000(time_t sec)
{
	const int32 sec_of_day = 3600 * 24;
	return (int32)(sec - get_sec_year_2000() - 1800) / sec_of_day;
}

/** ���ǻ�ȡ2000-01-01 00:00:00��������������������ϵͳʱ���仯 */
inline static int32 get_week_from_2000(time_t sec)
{
	const int32 sec_of_week = 3600 * 24 * 7;
	return (int32)(sec - get_sec_first_monday_of_year_2000()) / sec_of_week;
}

/** ��ȡ�����0ʱ0��0���ʱ�� */
inline static time_t get_begin_of_day(time_t t)
{
	struct tm store;
	LOCALTIME_SAFE(&t, &store);
	store.tm_hour = 0;
	store.tm_min = 0;
	store.tm_sec = 0;
	return mktime(&store);
}

/** ��ȡ�·��ӵ�ʱ�� */
inline static time_t get_next_minute(time_t now)
{
	return ((now / 60) * 60) + 60;
}

/** ��ȡ��Сʱ��ʱ�� */
inline static time_t get_next_hour(time_t now)
{
	return ((now / 3600) * 3600) + 3600;
}

/** ��ȡ��һ�ε�ָ��ʱ���� */
inline static time_t get_next_time(time_t now, int h, int m, int s)
{
	struct tm store;
	LOCALTIME_SAFE(&now, &store);
	store.tm_hour = h;
	store.tm_min = m;
	store.tm_sec = s;
	time_t sec = mktime(&store);

	if (now >= sec)
	{
		return sec + (3600 * 24);
	}
	else
	{
		return sec;
	}
}

/** ��ȡ��һ�ε�ָ����(0 - 3600 * 24) */
inline static time_t get_next_time(time_t now, int s)
{
	struct tm store;
	LOCALTIME_SAFE(&now, &store);
	store.tm_hour = 0;
	store.tm_min = 0;
	store.tm_sec = 0;
	time_t sec = mktime(&store);
	sec += s;
	if (now >= sec)
	{
		return sec + (3600 * 24);
	}
	else
	{
		return sec;
	}
}

/** ��ȡ��һ�ε�4ʱ0��0���ʱ�� */
inline static time_t get_next_refresh_time(time_t now, int h = 4, int min = 0)
{
	struct tm store;
	LOCALTIME_SAFE(&now, &store);
	store.tm_hour = h;
	store.tm_min = min;
	store.tm_sec = 0;
	time_t sec = mktime(&store);

	if (now >= sec)
	{
		return sec + (3600 * 24);
	}
	else
	{
		return sec;
	}
}

/* ������ڼ� 0������ 1����һ */
inline static int32 get_weekday(time_t t)
{
	struct tm store;
	LOCALTIME_SAFE(&t, &store);

	return store.tm_wday;
}

/* ��õڼ���Сʱ */
inline static int32 get_dayhour(time_t t)
{
	struct tm store;
	LOCALTIME_SAFE(&t, &store);

	return store.tm_hour;
}

/** ��ȡ�������������Χ��(0 <= sec < 24 * 3600) */
inline static int32 get_second_of_day(time_t t)
{
	return (get_second_from_2000(t) % (24 * 3600));
}

/*
 * ��õ����ʱ��
 */
inline static DayTime get_daytime(time_t t)
{
	struct tm store;
	LOCALTIME_SAFE(&t, &store);
	
	DayTime day_time;
	day_time._hour = store.tm_hour;
	day_time._min = store.tm_min;
	day_time._sec = store.tm_sec;

	return day_time;
}

//�Ƚ�a-bʱ���Ƿ�>=num��
#define compare_days(a, b, num)	((get_day_from_2000(a) - get_day_from_2000(b)) >= num ? true : false)
#define compare_two_day(a, b, op)	((get_day_from_2000(a) op get_day_from_2000(b)) ? true : false)
#define compare_two_hour(a, b, op)	((get_hour_from_2000(a) op get_hour_from_2000(b)) ? true : false)
#define compare_two_refresh(a, b, op)	((get_refresh_day_from_2000(a) op get_refresh_day_from_2000(b)) ? true : false)
#define compare_two_refresh_ex(a, b, op)	((get_refresh_day_ex_from_2000(a) op get_refresh_day_ex_from_2000(b)) ? true : false)
#define compare_two_week(a, b, op)	((get_week_from_2000(a) op get_week_from_2000(b)) ? true : false)

inline static void localtime_safe(time_t *sec, struct tm *store)
{
	//localtime_r(sec, store);
	LOCALTIME_SAFE(sec, store);
}


#endif /* GAMETIME_H_ */

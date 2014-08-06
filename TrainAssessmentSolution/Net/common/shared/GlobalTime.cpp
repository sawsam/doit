#include "GlobalTime.h"
#include "tools/ServerPacket.h"
#include "common/message/message_id.h"

using namespace sdk;

namespace common
{

GlobalTime::GlobalTime():_now(time(NULL)), _open_time(0)
{

}

GlobalTime::~GlobalTime()
{
	while (timer_queue_empty() == false)
	{
		timer_queue_pop();
	}
}

int32 GlobalTime::get_server_day_from_open()
{
	int32 open_day = get_day_from_2000(get_server_open_time());
	int32 now_day = get_day_from_2000(get_now_sec());

	if (open_day > now_day)
	{
		log_assert("false", __FILE__, __LINE__, __FUNCTION__);
		return now_day;
	}

	return now_day - open_day;
}

void GlobalTime::timer_queue_add( const SpecifyTimer &timer )
{
	if (timer._time < _now)
	{//过了当前时间
		return;
	}

	_timer_queue.push(new SpecifyTimer(timer));
}

bool GlobalTime::timer_queue_empty()
{
	return _timer_queue.empty();
}

const SpecifyTimer & GlobalTime::timer_queue_top()
{
	return *_timer_queue.top();
}

void GlobalTime::timer_queue_pop()
{
	SpecifyTimer *timer = _timer_queue.top();

	_timer_queue.pop();

	delete timer;

}

string GlobalTime::log_assert_string( const string &x, const string &file, int line, const string &func )
{
	stringstream ss;
	ss << "assert failed '" << x << "' : " << file << " " << line << " " << func;
	return ss.str();
}

void GlobalTime::log_assert( const string &x, const string &file, int line, const string &func )
{
	string content =  log_assert_string(x, file, line, func);

	cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	cout << "log assert : " << content << endl;
	cout << "++++++++++++++++++++++++++++++++++++++++++++++" << endl;

	log_to_db(content);
}

void GlobalTime::log_to_db( const string &content )
{
	//todo
}

}
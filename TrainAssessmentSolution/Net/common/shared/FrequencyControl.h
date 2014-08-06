#ifndef FREQUENCYCONTROL_H_
#define FREQUENCYCONTROL_H_

#include "global/base_inc.h"

namespace common
{
template<time_t SECOND, uint32 NUMBER>
class FrequencyControl
{
public:
	FrequencyControl()
	:_cur_sec(0),_count(0),_wait_sec(0)
	{

	}

	virtual ~FrequencyControl()
	{

	}

	bool check(time_t now)
	{
		if (NUMBER == 0)
		{
			return true;
		}

		if (now < _cur_sec)
		{
			_cur_sec = 0;
			_count = 0;
		}

		time_t sec = now - _cur_sec;

		if (sec >= SECOND)
		{
			_cur_sec = now;
			_count = 1;
			_wait_sec = 0;
			return true;
		}
		
		if (_count < NUMBER)
		{
			++_count;
			return true;
		}

		_wait_sec = SECOND - sec;
		return false;
	}

	time_t get_wait_sec()
	{
		return _wait_sec;
	}

private:
	time_t _cur_sec;
	uint32 _count;
	time_t _wait_sec;
};
}

#endif /* FREQUENCYCONTROL_H_ */
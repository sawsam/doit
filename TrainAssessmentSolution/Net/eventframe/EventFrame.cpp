#include  "EventFrame.h"

namespace sdk
{
EventFrame::EventFrame()
:_ef_working(false),_thread()
{
}

EventFrame::~EventFrame()
{
}

bool EventFrame::start()
{
	if (_ef_working)
	{
		return true;
	}

	_ef_working = true;
	_thread = ThreadPtr(new boost::thread(boost::bind(&EventFrame::ef_worker,this)));

	return true;
}

bool EventFrame::stop()
{
	if (!_ef_working)
	{
		return true;
	}

	_ef_working = false;
	_thread->join();

	return true;
}

void EventFrame::ef_worker()
{
	while (_ef_working)
	{
		user_process();
	}
}
}
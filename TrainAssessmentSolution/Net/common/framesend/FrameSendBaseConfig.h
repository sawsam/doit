#ifndef FRAMESENDBASECONFIG_H_
#define FRAMESENDBASECONFIG_H_

#include "enginebase/EngineBaseConfig.h"

using namespace sdk;

namespace common
{
#define DEF_FRAMESEND_INTERVAL 10	//10ms
#define DEF_FRAMESEND_BUFSIZE 51200	//50KBytes

/*
 * _timeout timer超时时间
 * _bufsize 发送缓冲大小
 * _print_timeval 打印发送信息的时间间隔，0为不打印，默认为0
 */
struct FramesendBaseConfig
{
	FramesendBaseConfig(const int32 timeout = DEF_FRAMESEND_INTERVAL,const uint16 bufsize = DEF_FRAMESEND_BUFSIZE,const int32 print_timeval = 0)
	: _timeout(timeout),_bufsize(bufsize),_print_timeval(print_timeval)
	{}
	FramesendBaseConfig(const FramesendBaseConfig & r)
	: _timeout(r._timeout), _bufsize(r._bufsize), _print_timeval(r._print_timeval),_config(r._config)
	{}
	FramesendBaseConfig& operator=(const FramesendBaseConfig& config)
	{
		_timeout = config._timeout;
		_bufsize = config._bufsize;
		_print_timeval = config._print_timeval;
		_config = config._config;
		return *this;
	}

	virtual ~FramesendBaseConfig(){}

	void set(int32 timeout, uint16 bufsize)
	{
		_timeout = timeout;
		_bufsize = bufsize;
	}

	void set_print_interval(int32 interval)
	{
		_print_timeval = interval;
	}

	int32 _timeout;
	uint16 _bufsize;
	int32 _print_timeval;
	sdk::EngineBaseConfig _config;
};
}

#endif /* FRAMESENDBASECONFIG_H_ */
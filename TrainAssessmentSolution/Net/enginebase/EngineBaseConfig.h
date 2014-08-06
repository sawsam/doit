#ifndef ENGINEBASECONFIG_H_
#define ENGINEBASECONFIG_H_

#include "global/base_inc.h"

namespace sdk
{
#define DEF_IDLE_WAIT 10 //us
#define DEF_SLOW_LOG_LIMIT 20000//us
struct EngineBaseConfig
{
	EngineBaseConfig():_idle_wait(DEF_IDLE_WAIT),_slow_log(false),_slow_log_limit(DEF_SLOW_LOG_LIMIT),_slow_log_filename("") {}

	EngineBaseConfig(const EngineBaseConfig& config)
	:_idle_wait(config._idle_wait),_slow_log(config._slow_log),
	_slow_log_limit(config._slow_log_limit),
	_slow_log_filename(config._slow_log_filename)
	{
	}
	EngineBaseConfig& operator=(const EngineBaseConfig& config)
	{
		_idle_wait = config._idle_wait;
		_slow_log = config._slow_log;
		_slow_log_limit = config._slow_log_limit;
		_slow_log_filename = config._slow_log_filename;
		return *this;
	}

	void set_idle_wait(uint32 idle_wait) { _idle_wait = idle_wait; }

	void enable_slow_log() { _slow_log = true; }

	void set_slow_log_limit(int64 limit) { _slow_log_limit = limit; }

	void set_slow_log_filename(string filename) { _slow_log_filename = filename; }

	uint32 _idle_wait;//空闲等待时间us
	bool _slow_log;//是否记录慢消息
	int64 _slow_log_limit;//慢消息临界时间us
	string _slow_log_filename;//慢消息记录文件名
};
}
#endif /* ENGINEBASECONFIG_H_ */
#ifndef COMMCONFIG_H_
#define COMMCONFIG_H_

#include "global/base_inc.h"

namespace sdk
{
#define DEF_IDLE_LIMIT 90000//ms
#define DEF_MAX_READ_UNIT 4096//bytes
#define DEF_MAX_READ_RATE (numeric_limits<unsigned int>::max)()
#define DEF_RECONNECT_INTERVAL 5000//ms

//连接配置
struct ConnectionConfig
{
	ConnectionConfig()
	:_idle_limit(DEF_IDLE_LIMIT),_max_read_unit(DEF_MAX_READ_UNIT),
	_max_read_rate(DEF_MAX_READ_RATE),_reconnect_interval(DEF_RECONNECT_INTERVAL)
	{
	}

	ConnectionConfig(const ConnectionConfig& config)
	:_idle_limit(config._idle_limit),_max_read_unit(config._max_read_unit),
	_max_read_rate(config._max_read_rate),_reconnect_interval(config._reconnect_interval)
	{
	}
	ConnectionConfig& operator=(const ConnectionConfig& config)
	{
		_idle_limit = config._idle_limit;
		_max_read_unit = config._max_read_unit;
		_max_read_rate = config._max_read_rate;
		_reconnect_interval = config._reconnect_interval;
		return *this;
	}

	uint32 _idle_limit;//空闲时间上限
	size_t _max_read_unit;//最大的读取单元
	uint32 _max_read_rate;//最大的读取频率
	uint32 _reconnect_interval;//重连间隔
};

//网络模块配置
class CommConfig
{
	friend class CommModule;
public:
	CommConfig()
	:_enable_acceptor(false),_listen_port(0)
	{
	}

	CommConfig(const CommConfig& config)
	:_enable_acceptor(config._enable_acceptor),_listen_port(config._listen_port),_conconfig(config._conconfig)
	{
	}
	CommConfig& operator=(const CommConfig& config)
	{
		_enable_acceptor = config._enable_acceptor;
		_listen_port = config._listen_port;
		_conconfig = config._conconfig;
		return *this;
	}

	void enable_acceptor() { _enable_acceptor = true; }

	void set_listen_port(uint16 port) { _listen_port = port; }

	void set_idle_limit(uint32 idle_limit) { _conconfig._idle_limit = idle_limit; }

	void set_max_read_unit(size_t max_read_unit) { _conconfig._max_read_unit = max_read_unit; }

	void set_max_read_rate(uint32 max_read_rate) { _conconfig._max_read_rate = max_read_rate; }

	void set_reconnect_interval(uint32 connect_interval) { _conconfig._reconnect_interval = connect_interval; }

private:
	bool _enable_acceptor;//是否启用接受器
	uint16 _listen_port;//监听端口
	ConnectionConfig _conconfig;//连接配置
};
}

#endif /* COMMCONFIG_H_ */

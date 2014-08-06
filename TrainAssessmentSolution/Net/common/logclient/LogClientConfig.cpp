/*
 * LogClientConfig.cpp
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#include "LogClientConfig.h"

LogClientConfig::LogClientConfig()
{
	_ip = "";
	_port = 0;
	_client_flag = 0;
	_pid = 0;

	_commconfig.set_idle_limit(0);
	_commconfig.set_max_read_unit(numeric_limits<uint16>::max());
}

LogClientConfig::LogClientConfig(const LogClientConfig& config)
{
	_ip = config._ip;
	_port = config._port;
	_client_flag = config._client_flag;
	_pid = config._pid;
}

LogClientConfig::~LogClientConfig()
{
	// TODO Auto-generated destructor stub
}



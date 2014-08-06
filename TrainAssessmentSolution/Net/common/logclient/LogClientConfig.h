/*
 * LogClientConfig.h
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#ifndef LOGCLIENTCONFIG_H_
#define LOGCLIENTCONFIG_H_

#include "common/serverbase/ServerBaseConfig.h"

class LogClientConfig : public common::ServerBaseConfig
{
public:
	string _ip;
	uint16 _port;
	int32 _client_flag;
	int32 _pid;

public:
	LogClientConfig();

	LogClientConfig(const LogClientConfig & config);

	virtual ~LogClientConfig();
};

#endif /* LOGCLIENTCONFIG_H_ */


/*
 * LogClient.h
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#ifndef LOGCLIENT_H_
#define LOGCLIENT_H_

#include "common/serverbase/ServerBase.h"
#include "Connector.h"
#include "LogRecord.h"

class LogClient
{
private:
	LogClientConfig _config;

	Connector * _conn;	//”ÎlogserverÕ®—∂

private:
	LogClient();

public:
	inline static LogClient * instance()
	{
		static LogClient instane;
		return &instane;
	}

	virtual ~LogClient();

public:
	void init_config(const LogClientConfig & config);

	bool start();

	bool stop();

public:
	void out_record(const LogRecord & record);

	void log_to_db(const string & log_content);
};

#endif /* LOGCLIENT_H_ */


/*
 * LogClient.cpp
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */
#include "LogClient.h"
#include "common/message/message_id.h"
#include "common/shared/datatype_define.h"

using namespace common;

LogClient::LogClient()
{
	_conn = NULL;
}

LogClient::~LogClient()
{
	if(_conn)
	{
		delete _conn;
	}

	//cout << "LogClient deconstruction." << endl; 
}

void LogClient::init_config(const LogClientConfig& config)
{
	_config = config;
}

bool LogClient::start()
{
	if(_conn == NULL)
	{
		_conn = new Connector(_config);
	}

	//ASSERT(_conn);

	if(_conn->is_running())
	{
		_conn->stop();
	}

	_conn->start();

	return true;
}

bool LogClient::stop()
{
	if(_conn->is_running())
	{
		_conn->stop();
	}

	return true;
}

void LogClient::out_record(const LogRecord & record)
{
	int32 now = (int32)time(NULL);

	ServerPacket * packet = new ServerPacket();
	packet->set_pid(now);
	packet->set_code(common::MCI_LOG_RECORD);
	packet->set_error(record._type);

	StreamWriter writer(packet->get_body());
	writer.write_uint16(record._contents.size());

	for(uint32 i = 0; i < record._contents.size(); ++i)
	{
		writer.write_string(record._contents[i]);
	}

	_conn->send_packet(packet);
}

void LogClient::log_to_db( const string & log_content )
{
	const int32 RI_PROGRAM_LOG = 1;

	LogRecord record;
	record.set_type(RI_PROGRAM_LOG);
	record.push_int32(common::MAP);
	record.push_string(log_content);

	LogClient::instance()->out_record(record);
}


/*
 * LogRecord.cpp
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#include "LogRecord.h"
#include "util/func_template.h"

LogRecord::LogRecord()
{
	this->_type = 0;
}

LogRecord::LogRecord(uint16 type)
{
	this->_type = type;
}

LogRecord::~LogRecord()
{
	// TODO Auto-generated destructor stub
}

uint16 LogRecord::get_type()
{
	return this->_type;
}

void LogRecord::set_type(uint16 type)
{
	this->_type = type;
}

void LogRecord::push_int8(int8 value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_int16(int16 value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);}

void LogRecord::push_int32(int32 value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_uint8(uint8 value)
{
	string content = sdk::to_string((int32)value);
	this->_contents.push_back(content);
}

void LogRecord::push_uint16(uint16 value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_uint32(uint32 value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_float(float value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_double(double value)
{
	string content = sdk::to_string(value);
	this->_contents.push_back(content);
}

void LogRecord::push_string(const string& value)
{
	this->_contents.push_back(value);
}

void LogRecord::clear()
{
	this->_contents.clear();
}


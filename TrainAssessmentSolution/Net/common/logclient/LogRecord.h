/*
 * LogRecord.h
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#ifndef LOGRECORD_H_
#define LOGRECORD_H_

#include "global/base_inc.h"

class LogClient;

class LogRecord
{
friend class LogClient;
private:
	uint16 _type;
	vector<string> _contents;

public:
	LogRecord();

	LogRecord(uint16 type);

	virtual ~LogRecord();

public:
	uint16 get_type();

	void set_type(uint16 type);

	void push_int8(int8 value);
	void push_int16(int16 value);
	void push_int32(int32 value);

	void push_uint8(uint8 value);
	void push_uint16(uint16 value);
	void push_uint32(uint32 value);

	void push_float(float value);
	void push_double(double value);
	void push_string(const string & value);

	void clear();
};

#endif /* LOGRECORD_H_ */


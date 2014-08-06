/*
 * StreamStreamReader::reader.cpp
 *
 *  Created on: 2010-10-9
 *      Author: work
 */

#include "StreamReader.h"

namespace sdk
{
StreamReader::StreamReader(StreamBase * stream)
{
	this->_stream = stream;
	this->_position = 0;
}

StreamReader::~StreamReader()
{
}

void StreamReader::step(uint32 step)
{
	this->_position += step;
}

int8 StreamReader::read_int8()
{
	int8 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(int8));
	step(length);
	return value;
}

int16 StreamReader::read_int16()
{
	int16 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(int16));
	step(length);
	return value;
}

int32 StreamReader::read_int32()
{
	int32 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(int32));
	step(length);
	return value;
}

int64 StreamReader::read_int64()
{
	int64 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(int64));
	step(length);
	return value;
}

uint8 StreamReader::read_uint8()
{
	uint8 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(uint8));
	step(length);
	return value;
}

uint16 StreamReader::read_uint16()
{
	uint16 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(uint16));
	step(length);
	return value;
}

uint32 StreamReader::read_uint32()
{
	uint32 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(uint32));
	step(length);
	return value;
}

uint64 StreamReader::read_uint64()
{
	uint64 value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(uint64));
	step(length);
	return value;
}

float StreamReader::read_float()
{
	float value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(float));
	step(length);
	return value;
}

double StreamReader::read_double()
{
	double value = 0;
	uint32 length = this->_stream->read(this->_position, (void *) &value, sizeof(double));
	step(length);
	return value;
}

std::string StreamReader::read_string()
{
	uint16 str_length = 0;
	uint32 length = 0;
	length = this->_stream->read(this->_position, (void *) &str_length, sizeof(uint16));
	step(length);

	if (length == 0)
	{
		return "";
	}

	if (this->_stream->size() >= this->_position + str_length)
	{
		std::string value(this->_stream->get_buffer() + this->_position, str_length);
		step(str_length);
		return value;
	}

	return "";
}

uint32 StreamReader::read_bytes(void * value, uint32 size)
{
	uint32 length = this->_stream->read(this->_position, value, size);
	step(length);

	return length;
}

uint32 & StreamReader::position()
{
	return this->_position;
}
}

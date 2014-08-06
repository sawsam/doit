/*
 * StreamWriter.cpp
 *
 *  Created on: 2010-10-9
 *      Author: work
 */

#include "StreamWriter.h"

namespace sdk
{
StreamWriter::StreamWriter(StreamBase * stream)
{
	this->_stream = stream;
	this->_position = 0;
}

StreamWriter::StreamWriter(StreamBase * stream, bool append)
{
	this->_stream = stream;
	this->_position = append ? stream->size() : 0;
}

StreamWriter::~StreamWriter()
{

}

uint32 StreamWriter::length()
{
	return this->_position;
}

const char * StreamWriter::get_buffer()
{
	return this->_stream->get_buffer();
}

void StreamWriter::step(uint32 step)
{
	this->_position+=step;
}

void StreamWriter::write_int8(int8 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(int8));
	step(length);
}

void StreamWriter::write_int16(int16 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(int16));
	step(length);
}

void StreamWriter::write_int32(int32 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(int32));
	step(length);
}

void StreamWriter::write_int64(int64 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(int64));
	step(length);
}

void StreamWriter::write_uint8(uint8 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(uint8));
	step(length);
}

void StreamWriter::write_uint16(uint16 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(uint16));
	step(length);
}

void StreamWriter::write_uint32(uint32 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(uint32));
	step(length);
}

void StreamWriter::write_uint64(uint64 value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(uint64));
	step(length);
}

void StreamWriter::write_float(float value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(float));
	step(length);
}

void StreamWriter::write_double(double value)
{
	uint32 length = this->_stream->write(this->_position,(const void *)&value,sizeof(double));
	step(length);
}

void StreamWriter::write_string(const std::string & value)
{
	uint16 str_length = (uint16)value.length();
	uint32 length = this->_stream->write(this->_position,(const void *)&str_length,sizeof(uint16));
	step(length);
	length = this->_stream->write(this->_position,(const void *)value.data(),str_length);
	step(str_length);
}

void StreamWriter::write_bytes(const void *value, uint32 size)
{
	uint32 length = this->_stream->write(this->_position,value,size);
	step(length);
}

uint32 & StreamWriter::position()
{
	return this->_position;
}

void StreamWriter::write(int8 value)
{
	write_int8(value);
}

void StreamWriter::write(int16 value)
{
	write_int16(value);
}

void StreamWriter::write(int32 value)
{
	write_int32(value);
}

void StreamWriter::write(int64 value)
{
	write_int64(value);
}

void StreamWriter::write(uint8 value)
{
	write_uint8(value);
}

void StreamWriter::write(uint16 value)
{
	write_uint16(value);
}

void StreamWriter::write(uint32 value)
{
	write_uint32(value);
}

void StreamWriter::write(uint64 value)
{
	write_uint64(value);
}

void StreamWriter::write(float value)
{
	write_float(value);
}

void StreamWriter::write(double value)
{
	write_double(value);
}

void StreamWriter::write(const std::string & value)
{
	write_string(value);
}
}

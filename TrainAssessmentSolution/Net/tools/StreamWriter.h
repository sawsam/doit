/*
 * StreamWriter.h
 *
 *  Created on: 2010-10-9
 *      Author: work
 */

#ifndef STREAMWRITER_H_
#define STREAMWRITER_H_

#include "StreamBase.h"
#include "Noncopyable.h"

namespace sdk
{
class StreamWriter : public Noncopyable
{

protected:
	uint32 _position;
	StreamBase * _stream;

protected:
	void step(uint32 step);

public:
	StreamWriter(StreamBase * stream);
	StreamWriter(StreamBase * stream, bool append);
	virtual ~StreamWriter();

public:

	uint32 length();

	const char * get_buffer();

	void write_int8(int8 value);

	void write_int16(int16 value);

	void write_int32(int32 value);

	void write_int64(int64 value);

	void write_uint8(uint8 value);

	void write_uint16(uint16 value);

	void write_uint32(uint32 value);

	void write_uint64(uint64 value);

	void write_float(float value);

	void write_double(double value);

	void write_string(const std::string & value);

	void write_bytes(const void *value, uint32 size);

	uint32 & position();

	void write(int8 value);

	void write(int16 value);

	void write(int32 value);

	void write(int64 value);

	void write(uint8 value);

	void write(uint16 value);

	void write(uint32 value);

	void write(uint64 value);

	void write(float value);

	void write(double value);

	void write(const std::string & value);
};
}

#endif /* STREAMWRITER_H_ */

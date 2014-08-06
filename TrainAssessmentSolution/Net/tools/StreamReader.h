/*
 * StreamReader.h
 *
 *  Created on: 2010-10-9
 *      Author: work
 */

#ifndef STREAMREADER_H_
#define STREAMREADER_H_

#include "StreamBase.h"
#include "Noncopyable.h"

namespace sdk
{
class StreamReader : public Noncopyable
{

protected:

	uint32 _position;
	StreamBase * _stream;

protected:
	void step(uint32 step);

public:
	StreamReader(StreamBase * stream);
	virtual ~StreamReader();

public:
	int8 read_int8();

	int16 read_int16();

	int32 read_int32();

	int64 read_int64();

	uint8 read_uint8();

	uint16 read_uint16();

	uint32 read_uint32();

	uint64 read_uint64();

	float read_float();

	double read_double();

	std::string read_string();

	uint32 read_bytes(void * value, uint32 size);

	uint32 & position();
};
}

#endif /* STREAMREADER_H_ */

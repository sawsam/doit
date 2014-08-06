/*
 * MessageStream.h
 *
 *  Created on: Nov 3, 2010
 *      Author: work
 */

#ifndef MESSAGESTREAM_H_
#define MESSAGESTREAM_H_

#include "global/base_inc.h"
#include "tools/PacketHead.h"
#include "tools/StreamBase.h"

namespace sdk
{
class MessageStream : public StreamBase
{
private:
	std::string _buffer;
	SSPacketHead _head;

public:
	MessageStream(uint32 size = 1024);

	MessageStream(const MessageStream &obj);

	virtual ~MessageStream();

public:
	virtual uint32 size();

	virtual uint32 write(uint32 position, const void * ptr, uint32 length);

	virtual uint32 read(uint32 position, void * ptr, uint32 length);

	virtual const char * get_buffer() const;

	void reset();

	const char * get_long_buffer();

	uint32 get_long_size();

	const char * get_short_buffer();

	uint32 get_short_size();

	inline uint64 get_uid() { return _head._uid; }

	void set_uid(uint64 uid);

	inline int32 get_pid() { return _head._pid; }

	void set_pid(int32 pid);

	inline uint16 get_code() { return _head._id; }

	void set_code(uint16 id);

	inline uint16 get_error() { return _head._ec; }

	void set_error(uint16 error);
};
}

#endif /* MESSAGESTREAM_H_ */

/*
 * MessageStream.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: work
 */

#include "MessageStream.h"

namespace sdk
{
MessageStream::MessageStream(uint32 size) : _buffer(size + sizeof(_head), 0)
{
	_head._uid = 0;
	_head._pid = 0;
	_head._id = 0;
	_head._ec = 0;

	_buffer.resize(sizeof(_head));
}

MessageStream::MessageStream(const MessageStream &obj) : _buffer(obj._buffer), _head(obj._head)
{

}

MessageStream::~MessageStream()
{

}

uint32 MessageStream::size()
{
	return _buffer.size() - sizeof(_head);
}

uint32 MessageStream::write(uint32 position, const void * ptr, uint32 length)
{
	_buffer.replace(position + sizeof(_head), length, (const char *)ptr, length);
	return length;
}

uint32 MessageStream::read(uint32 position, void * ptr, uint32 length)
{
	return _buffer.copy((char *)ptr, length, position + sizeof(_head));
}

const char * MessageStream::get_buffer() const
{
	return _buffer.data() + sizeof(_head);
}

void MessageStream::reset()
{
	if (_buffer.size() > sizeof(_head))
	{
		_buffer.resize(sizeof(_head));
	}
}

const char * MessageStream::get_long_buffer()
{
	return _buffer.data();
}

uint32 MessageStream::get_long_size()
{
	return _buffer.size();
}

const char * MessageStream::get_short_buffer()
{
	return _buffer.data() + (sizeof(SSPacketHead) - sizeof(CSPacketHead));
}

uint32 MessageStream::get_short_size()
{
	return _buffer.size() - (sizeof(SSPacketHead) - sizeof(CSPacketHead));
}

void MessageStream::set_uid(uint64 uid)
{
	if (_head._uid != uid)
	{
		_head._uid = uid;
		_buffer.replace(0, sizeof(_head), (const char *)&_head, sizeof(_head));
	}
}

void MessageStream::set_pid(int32 pid)
{
	if (_head._pid != pid)
	{
		_head._pid = pid;
		_buffer.replace(0, sizeof(_head), (const char *)&_head, sizeof(_head));
	}
}

void MessageStream::set_code(uint16 id)
{
	if (_head._id != id)
	{
		_head._id = id;
		_buffer.replace(0, sizeof(_head), (const char *)&_head, sizeof(_head));
	}
}

void MessageStream::set_error(uint16 error)
{
	if (_head._ec != error)
	{
		_head._ec = error;
		_buffer.replace(0, sizeof(_head), (const char *)&_head, sizeof(_head));
	}
}
}
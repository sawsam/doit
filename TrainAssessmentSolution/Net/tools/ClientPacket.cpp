/*
 * ClientPacket.cpp
 *
 *  Created on: Oct 12, 2010
 *      Author: work
 */

#include "ClientPacket.h"
#include "ServerPacket.h"

namespace sdk
{
ClientPacket::ClientPacket(uint32 bodysize) :
	_msg(bodysize), _fd(-1), _sn(0)
{

}

ClientPacket::ClientPacket(int fd, int32 sn, char * buf, int32 size) :
	_msg(size), _fd(fd), _sn(sn)
{
	parse((const char *)buf, size);
}

ClientPacket::ClientPacket(const ServerPacket &obj) :
	_msg(obj._msg), _fd(obj._fd), _sn(obj._sn)
{

}

ClientPacket::~ClientPacket()
{

}

StreamBase * ClientPacket::get_body()
{
	return &_msg;
}

bool ClientPacket::parse(const char *buf, uint32 size)
{
	if (size >= sizeof(CSPacketHead))
	{
		_msg.reset();

		CSPacketHead *cshead = (CSPacketHead *)buf;

		_msg.set_code(cshead->_id);
		_msg.set_error(cshead->_ec);

		if (size > sizeof(CSPacketHead))
		{
			_msg.write(0, buf + sizeof(CSPacketHead), size - sizeof(CSPacketHead));
		}

		return true;
	}

	return false;
}

const char * ClientPacket::get_msg_buffer()
{
	return _msg.get_short_buffer();
}

uint32 ClientPacket::get_msg_size()
{
	return _msg.get_short_size();
}
}

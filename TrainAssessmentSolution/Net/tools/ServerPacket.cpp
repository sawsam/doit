/*
 * ServerPacket.cpp
 *
 *  Created on: Oct 12, 2010
 *      Author: work
 */

#include "ServerPacket.h"
#include "ClientPacket.h"

namespace sdk
{
ServerPacket::ServerPacket(const ClientPacket &obj) : _msg(obj._msg), _fd(obj._fd), _sn(obj._sn)
{

}

bool ServerPacket::parse(const char *buf, uint32 size)
{
	if (size >= sizeof(SSPacketHead))
	{
		_msg.reset();

		SSPacketHead *sshead = (SSPacketHead *)buf;

		_msg.set_uid(sshead->_uid);
		_msg.set_pid(sshead->_pid);
		_msg.set_code(sshead->_id);
		_msg.set_error(sshead->_ec);

		if (size > sizeof(SSPacketHead))
		{
			_msg.write(0, buf + sizeof(SSPacketHead), size - sizeof(SSPacketHead));
		}

		return true;
	}

	return false;
}

ServerPacket * ServerPacket::clone()
{
	ServerPacket *sp = new ServerPacket(*this);
	return sp;
}
}

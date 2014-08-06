/*
 * ServerPacket.h
 *
 *  Created on: Oct 12, 2010
 *      Author: work
 */

#ifndef SERVERPACKET_H_
#define SERVERPACKET_H_

#include "global/base_inc.h"
#include "tools/PacketBase.h"
#include "MessageStream.h"
#include "tools/StreamReader.h"
#include "tools/StreamWriter.h"

namespace sdk
{
class ClientPacket;
class ServerPacket : public PacketBase
{
	friend class ClientPacket;
private:
	MessageStream _msg;

	int _fd;
	int32 _sn;

public:
	ServerPacket(uint32 bodysize = MBL_256_BYTE)
	: _msg(bodysize), _fd(-1), _sn(0)
	{ }
	ServerPacket(int fd, int32 sn, char * buf, int32 size)
	: _msg(size), _fd(fd), _sn(sn)
	{ parse((const char *)buf, size); }

	explicit ServerPacket(const ServerPacket &obj)
	: _msg(obj._msg), _fd(obj._fd), _sn(obj._sn)
	{ }

	explicit ServerPacket(const ClientPacket &obj);

	ServerPacket(int fd, int32 sn, uint64 uid, int32 pid, uint16 code, uint16 ec)
	: _fd(fd), _sn(sn)
	{
		_msg.set_uid(uid);
		_msg.set_pid(pid);
		_msg.set_code(code);
		_msg.set_error(ec);
	}

	virtual ~ServerPacket() { }

	inline virtual uint16 get_code(){return _msg.get_code();}

	inline void set_code(uint16 code){_msg.set_code(code);}

	inline uint16 get_error(){return _msg.get_error();}

	inline void set_error(uint16 error){_msg.set_error(error);}

	inline int32 get_pid(){return _msg.get_pid();}

	inline void set_pid(int32 pid){_msg.set_pid(pid);}

	inline uint64 get_uid(){return _msg.get_uid();}

	inline void set_uid(uint64 uid){_msg.set_uid(uid);}

	inline int get_fd(){return _fd;}

	inline void set_fd(int fd){_fd = fd;}

	inline int32 get_sn(){return _sn;}

	inline void set_sn(int32 sn){_sn = sn;}

	inline StreamBase * get_body(){return &_msg;}

	virtual bool parse(const char *buf, uint32 size);

	inline virtual const char * get_msg_buffer(){return _msg.get_long_buffer();}

	inline virtual uint32 get_msg_size(){return _msg.get_long_size();}

	ServerPacket * clone();
};
}

#endif /* SERVERPACKET_H_ */

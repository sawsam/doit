/*
 * ClientPacket.h
 *
 *  Created on: Oct 12, 2010
 *      Author: work
 */

#ifndef CLIENTPACKET_H_
#define CLIENTPACKET_H_

#include "global/base_inc.h"
#include "tools/PacketBase.h"
#include "MessageStream.h"
#include "tools/StreamReader.h"
#include "tools/StreamWriter.h"

namespace sdk
{
class ServerPacket;
class ClientPacket : public PacketBase
{
	friend class ServerPacket;
private:
	MessageStream _msg;

	int _fd;
	int32 _sn;

public:
	ClientPacket(uint32 bodysize = MBL_256_BYTE);

	ClientPacket(int fd, int32 sn, char * buf, int32 size);

	explicit ClientPacket(const ServerPacket &obj);

	virtual ~ClientPacket();

	inline virtual uint16 get_code() { return _msg.get_code(); }

	inline void set_code(uint16 code) { _msg.set_code(code); }

	inline uint16 get_error() { return _msg.get_error(); }

	inline void set_error(uint16 error) { _msg.set_error(error); }

	inline int get_fd() { return _fd; }

	inline void set_fd(int fd) { _fd = fd; }

	inline int32 get_sn() { return _sn; }

	inline void set_sn(int32 sn) { _sn = sn; }

	StreamBase * get_body();

	virtual bool parse(const char *buf, uint32 size);

	virtual const char * get_msg_buffer();

	virtual uint32 get_msg_size();
};
}

#endif /* CLIENTPACKET_H_ */

/*
 * PacketBase.h
 *
 *  Created on: Oct 9, 2010
 *      Author: work
 */

#ifndef PACKETBASE_H_
#define PACKETBASE_H_

#include "global/base_inc.h"

namespace sdk
{
class PacketBase
{
public:
	PacketBase();

	virtual ~PacketBase();

public:
	virtual uint16 get_code() = 0;

	virtual bool parse(const char *buf, uint32 size) = 0;

	virtual const char * get_msg_buffer() = 0;

	virtual uint32 get_msg_size() = 0;
};
}

#endif /* PACKETBASE_H_ */

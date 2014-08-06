/*
 * StreamBase.h
 *
 *  Created on: 2010-10-9
 *      Author: work
 */

#ifndef STREAMBASE_H_
#define STREAMBASE_H_

#include "global/base_inc.h"

namespace sdk
{
class StreamBase
{
public:
	virtual ~StreamBase() { }

	virtual uint32 size() = 0;

	virtual uint32 write(uint32 position, const void * ptr, uint32 length) = 0;

	virtual uint32 read(uint32 position, void * ptr, uint32 length) = 0;

	virtual const char * get_buffer() const = 0;
};
}


#endif /* STREAMBASE_H_ */

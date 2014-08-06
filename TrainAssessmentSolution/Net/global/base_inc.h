#ifndef BASE_INC_H_
#define BASE_INC_H_

/*
	 涉及到UUGame工程包含define_inc.h即可
	 其他工程可以直接包含base_inc.h
*/

#include <assert.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/crc.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "define_inc.h"

static const boost::shared_ptr<void> NullPtr;


#endif /* BASE_INC_H_ */
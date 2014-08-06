#ifndef BASE_INC_H_
#define BASE_INC_H_

/*
	 �漰��UUGame���̰���define_inc.h����
	 �������̿���ֱ�Ӱ���base_inc.h
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
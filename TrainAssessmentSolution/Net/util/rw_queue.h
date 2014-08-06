#ifndef RW_QUEUE_H_
#define RW_QUEUE_H_

#include "func_template.h"

namespace sdk
{
template <class T>
class rw_queue
{
public:
	rw_queue()
	:_qr(&_q[0]),_qw(&_q[1])
	{
	}
	virtual ~rw_queue()
	{
		clean();
	}

	inline queue<T> * get_rq()
	{
		return _qr;
	}
	inline size_t rq_size()
	{
		return _qr->size();
	}
	inline void push(const T & m)
	{
		_mtx.lock();
		_qw->push(m);
		_mtx.unlock();
	}
	inline void swap()
	{
		queue<T> * t = _qr;
		_mtx.lock();
		_qr = _qw;
		_qw = t;
		_mtx.unlock();
	}
	void clean()
	{
		_mtx.lock();
		queue_clean_and_free(_q[0]);
		queue_clean_and_free(_q[1]);
		_mtx.unlock();
	}
private:
	queue<T> _q[2];
	queue<T> * _qr;
	queue<T> * _qw;
	boost::mutex _mtx;
};
}

#endif /* RW_QUEUE_H_ */

/*
 * Noncopyable.h
 *
 *  Created on: Jan 4, 2011
 *      Author: work
 */

#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

namespace sdk
{
class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}
private:
	Noncopyable(const Noncopyable&);
	const Noncopyable& operator=(const Noncopyable&);
};
}

#endif /* NONCOPYABLE_H_ */

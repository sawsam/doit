#ifndef EVENTFRAME_H_
#define EVENTFRAME_H_

#include "global/base_inc.h"

namespace sdk
{
class EventFrame
{
	typedef boost::shared_ptr<boost::thread> ThreadPtr;
public:
	EventFrame();

	virtual ~EventFrame();

	/*
		描述：开启线程
		返回：true开启成功，false开启失败
	*/
	bool start();

	/*
		描述：关闭线程
		返回：true关闭成功，false关闭失败
	*/
	bool stop();

private:
	/*
		描述：挂钩函数，把需要做的操作放在此函数内，注意适当睡眠防止空跑 
	*/
	virtual void user_process() = 0;

	void ef_worker();

protected:
	bool _ef_working;//线程是否在工作
	ThreadPtr _thread;//线程
};
}

#endif /* EVENTFRAME_H_ */
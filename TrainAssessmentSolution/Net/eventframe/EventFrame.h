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
		�����������߳�
		���أ�true�����ɹ���false����ʧ��
	*/
	bool start();

	/*
		�������ر��߳�
		���أ�true�رճɹ���false�ر�ʧ��
	*/
	bool stop();

private:
	/*
		�������ҹ�����������Ҫ���Ĳ������ڴ˺����ڣ�ע���ʵ�˯�߷�ֹ���� 
	*/
	virtual void user_process() = 0;

	void ef_worker();

protected:
	bool _ef_working;//�߳��Ƿ��ڹ���
	ThreadPtr _thread;//�߳�
};
}

#endif /* EVENTFRAME_H_ */
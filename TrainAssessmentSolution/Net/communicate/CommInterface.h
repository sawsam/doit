#ifndef COMMINTERFACE_H_
#define COMMINTERFACE_H_

#include "comm_data_struct.h"

namespace sdk
{
//����ģ��ӿ�
class CommInterface
{
public:
	CommInterface() {}
	virtual ~CommInterface() {}

	/*
		������֪ͨ�ϲ����Ӵ�
		������fd���Ӿ����sn�������
	*/
	virtual void notice_connection_open(int fd,int sn) {}

	/*
		������֪ͨ�ϲ����ӶϿ�
		������fd���Ӿ����sn�������
	*/
	virtual void notice_connection_close(int fd,int sn) {}

	/*
		������֪ͨ�ϲ���Ϣ����
		������fd���Ӿ����sn������ţ�msg��Ϣ���壬len��Ϣ����
	*/
	virtual void notice_message_arrive(int fd,int sn,char* msg,size_t len) {}

	/*
		������֪ͨ�ϲ����ӷ�����ʧ��
		������sn�������
	*/
	virtual void notice_connect_error(int sn) {}

	/*
		�������������ӷ�����
		������ci������Ϣ
	*/
	virtual void request_connect_server(const ConnectInfo& ci) {}

	/*
		����������ر�����
		������fd���Ӿ����sn�������
	*/
	virtual void request_close_connection(int fd,int sn) {}

	/*
		��������������Ϣ
		������fd���Ӿ����sn������ţ�msg��Ϣ���壬len��Ϣ����
	*/
	virtual void request_send_message(int fd,int sn,const char* msg,size_t len) {}

	/*
		������֪ͨ�ϲ�һ����Ϣ�Ѿ�������ɣ����ڶ����ӣ�������Ϣ��Ͽ����ӣ�
	*/
	virtual void notice_message_send(int fd,int sn) {}
};
}

#endif /* COMMINTERFACE_H_ */
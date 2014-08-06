#ifndef COMMMODULE_H_
#define COMMMODULE_H_

#include "eventframe/EventFrame.h"
#include "Connection.h"
#include "util/func_template.h"

namespace sdk
{
class CommModule : public EventFrame, public CommInterface
{
public:
	CommModule(const CommConfig& config,CommInterface* owner);

	~CommModule();

	/*
		������������������ģ��
		���أ�true�����ɹ���false����ʧ��
	*/
	bool start();

	/*
		�������ر�����ģ��
		���أ�true�رճɹ���false�ر�ʧ��
	*/
	bool stop();

	virtual void notice_connection_open(int fd,int sn);

	virtual void notice_connection_close(int fd,int sn);

	virtual void notice_message_arrive(int fd,int sn,char* msg,size_t len);

	virtual void notice_connect_error(int sn);

	virtual void request_connect_server(const ConnectInfo& ci);

	virtual void request_close_connection(int fd,int sn);

	virtual void request_send_message(int fd,int sn,const char* msg,size_t len);

	virtual void notice_message_send(int fd,int sn);

private:
	inline int get_sn() { return --_sn; }

	void do_connect_server(ConnectInfo ci);

	void do_close_connection(int fd,int sn);

	void do_send_message(MsgSendPtr msg);

	virtual void user_process();

	void start_accept();

	void handle_accept(ConnectionPtr conn,const boost::system::error_code& error);

private:
	CommConfig _config;//����ģ������
	CommInterface* _owner;//�ϲ㼴����ģ���ӵ����
	boost::asio::io_service _io_service;//io_service
	boost::asio::io_service::work _work;//��֤io_service.run()�����˳�
	boost::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;//������
	std::hash_map<int64,ConnectionPtr> _connected;//�Ѿ����ӵ��б�
	std::hash_map<int,ConnectionPtr> _connecting;//�������ӵ��б�
	int _sn;//�������
};
}

#endif /* COMMMODULE_H_ */
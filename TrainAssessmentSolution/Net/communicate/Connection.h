#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "CommInterface.h"
#include "CommConfig.h"

namespace sdk
{
class Connection : public boost::enable_shared_from_this<Connection>
{
public:
	Connection(ConnectionConfig& config,CommInterface* owner,boost::asio::io_service& io_service);

	~Connection();

	/*
		������������ӵ��׽���
		���أ��׽�������
	*/
	inline boost::asio::ip::tcp::socket& socket() { return _socket; }

	/*
		�������������ӣ������Ӳ�����һ����ʱ��������Զ�����
		������ci������Ϣ
	*/
	void connect(const ConnectInfo& ci);

	/*
		�������������ӣ���������ȡ����������������timer����м��timer
	*/
	void start();

	/*
		�������ر����Ӳ�֪ͨ�ϲ�
	*/
	void stop();

	/*
		������������Ϣ
		������msg���ͻ���
	*/
	void send(SendBuf* msg);

	inline void set_fd(int fd) { _fd = fd; }

	inline int get_fd() { return _fd; }

	inline void set_sn(int sn) { _sn = sn; }

	inline int get_sn() { return _sn; }

private:
	/*
		��������ȡ��Ϣͷ����ɺ�Ļص�����
	*/
	void handle_read_header(const boost::system::error_code& error);

	/*
		��������ȡ��Ϣ����ɺ�Ļص�����
	*/
	void handle_read_body(const boost::system::error_code& error);

	/*
		������������ɺ�Ļص�����
	*/
	void handle_write(const boost::system::error_code& error);

	/*
		������������ɺ�Ļص�����
	*/
	void handle_connect(ConnectInfo ci,const boost::system::error_code& error);

	/*
		���������ӳ�ʱ��Ļص�����
	*/
	void handle_connect_timeout(ConnectInfo ci,const boost::system::error_code& error);

	/*
		����������timer��ʱ��Ļص�����
	*/
	void handle_reconect_timeout(ConnectInfo ci,const boost::system::error_code& error);

	/*
		���������м��timer��ʱ��Ļص�����
	*/
	void handle_idle(const boost::system::error_code& error);

	/*
		��������������timer��ʱ��Ļص�����
	*/
	void handle_heartbeat(const boost::system::error_code& error);

private:
	ConnectionConfig& _config;//��������
	CommInterface* _owner;//�ϲ㼴����ӵ����
	boost::asio::ip::tcp::socket _socket;//�׽���
	boost::asio::deadline_timer _idle_timer;//��������timer����м��timer
	boost::asio::deadline_timer _connect_timer;//���ӳ�ʱtimer
	boost::asio::deadline_timer _reconnect_timer;//����timer
	ReceiveBuf _rbuf;//���ջ���
	std::queue<SendBuf*> _send_q;//���Ͷ���
	int _fd;//���Ӿ��
	int _sn;//�������

	//ͳ����Ϣ
	unsigned int _last_recv_count;
	time_t _last_recv_time;
	unsigned int _send_count;
	unsigned int _recv_count;
};
typedef boost::shared_ptr<Connection> ConnectionPtr;
}

#endif /* CONNECTION_H_ */
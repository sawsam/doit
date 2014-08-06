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
		描述：获得连接的套接字
		返回：套接字引用
	*/
	inline boost::asio::ip::tcp::socket& socket() { return _socket; }

	/*
		描述：发起连接，若连接不上在一定的时间间隔后会自动重连
		参数：ci连接信息
	*/
	void connect(const ConnectInfo& ci);

	/*
		描述：开启连接，即开启读取，并设置心跳发送timer或空闲检测timer
	*/
	void start();

	/*
		描述：关闭连接并通知上层
	*/
	void stop();

	/*
		描述：发送消息
		参数：msg发送缓冲
	*/
	void send(SendBuf* msg);

	inline void set_fd(int fd) { _fd = fd; }

	inline int get_fd() { return _fd; }

	inline void set_sn(int sn) { _sn = sn; }

	inline int get_sn() { return _sn; }

private:
	/*
		描述：读取消息头部完成后的回调函数
	*/
	void handle_read_header(const boost::system::error_code& error);

	/*
		描述：读取消息体完成后的回调函数
	*/
	void handle_read_body(const boost::system::error_code& error);

	/*
		描述：发送完成后的回调函数
	*/
	void handle_write(const boost::system::error_code& error);

	/*
		描述：连接完成后的回调函数
	*/
	void handle_connect(ConnectInfo ci,const boost::system::error_code& error);

	/*
		描述：连接超时后的回调函数
	*/
	void handle_connect_timeout(ConnectInfo ci,const boost::system::error_code& error);

	/*
		描述：重连timer超时后的回调函数
	*/
	void handle_reconect_timeout(ConnectInfo ci,const boost::system::error_code& error);

	/*
		描述：空闲检测timer超时后的回调函数
	*/
	void handle_idle(const boost::system::error_code& error);

	/*
		描述：心跳发送timer超时后的回调函数
	*/
	void handle_heartbeat(const boost::system::error_code& error);

private:
	ConnectionConfig& _config;//连接配置
	CommInterface* _owner;//上层即连接拥有者
	boost::asio::ip::tcp::socket _socket;//套接字
	boost::asio::deadline_timer _idle_timer;//心跳发送timer或空闲检测timer
	boost::asio::deadline_timer _connect_timer;//连接超时timer
	boost::asio::deadline_timer _reconnect_timer;//重连timer
	ReceiveBuf _rbuf;//接收缓冲
	std::queue<SendBuf*> _send_q;//发送队列
	int _fd;//连接句柄
	int _sn;//连接序号

	//统计信息
	unsigned int _last_recv_count;
	time_t _last_recv_time;
	unsigned int _send_count;
	unsigned int _recv_count;
};
typedef boost::shared_ptr<Connection> ConnectionPtr;
}

#endif /* CONNECTION_H_ */
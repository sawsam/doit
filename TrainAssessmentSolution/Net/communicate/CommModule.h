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
		描述：开启网络连接模块
		返回：true开启成功，false开启失败
	*/
	bool start();

	/*
		描述：关闭网络模块
		返回：true关闭成功，false关闭失败
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
	CommConfig _config;//网络模块配置
	CommInterface* _owner;//上层即网络模块的拥有者
	boost::asio::io_service _io_service;//io_service
	boost::asio::io_service::work _work;//保证io_service.run()不会退出
	boost::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;//接受器
	std::hash_map<int64,ConnectionPtr> _connected;//已经连接的列表
	std::hash_map<int,ConnectionPtr> _connecting;//正在连接的列表
	int _sn;//连接序号
};
}

#endif /* COMMMODULE_H_ */
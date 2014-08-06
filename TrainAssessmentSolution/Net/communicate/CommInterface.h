#ifndef COMMINTERFACE_H_
#define COMMINTERFACE_H_

#include "comm_data_struct.h"

namespace sdk
{
//网络模块接口
class CommInterface
{
public:
	CommInterface() {}
	virtual ~CommInterface() {}

	/*
		描述：通知上层连接打开
		参数：fd连接句柄，sn连接序号
	*/
	virtual void notice_connection_open(int fd,int sn) {}

	/*
		描述：通知上层连接断开
		参数：fd连接句柄，sn连接序号
	*/
	virtual void notice_connection_close(int fd,int sn) {}

	/*
		描述：通知上层消息到达
		参数：fd连接句柄，sn连接序号，msg消息缓冲，len消息长度
	*/
	virtual void notice_message_arrive(int fd,int sn,char* msg,size_t len) {}

	/*
		描述：通知上层连接服务器失败
		参数：sn连接序号
	*/
	virtual void notice_connect_error(int sn) {}

	/*
		描述：请求连接服务器
		参数：ci连接信息
	*/
	virtual void request_connect_server(const ConnectInfo& ci) {}

	/*
		描述：请求关闭连接
		参数：fd连接句柄，sn连接序号
	*/
	virtual void request_close_connection(int fd,int sn) {}

	/*
		描述：请求发送消息
		参数：fd连接句柄，sn连接序号，msg消息缓冲，len消息长度
	*/
	virtual void request_send_message(int fd,int sn,const char* msg,size_t len) {}

	/*
		描述：通知上层一个消息已经发送完成（用于短连接：发送消息后断开连接）
	*/
	virtual void notice_message_send(int fd,int sn) {}
};
}

#endif /* COMMINTERFACE_H_ */
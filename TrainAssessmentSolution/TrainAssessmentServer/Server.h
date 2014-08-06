#ifndef SERVER_H_
#define SERVER_H_

#include "common/serverbase/ServerBase.h"
#include "common/message/message_id.h"
#include "common/message/error_code.h"
#include "UserInfo.h"
#include "DataStruct.h"

using namespace common;

class Server : public ServerBase
{
	class PrintTimer : public TimerHandler
	{
	public:
		PrintTimer(Server* server);
		~PrintTimer();

		virtual TimerHandleResult on_timer();

	private:
		Server* _server;
	};
public:
	Server(const ServerBaseConfig& config);
	~Server() ;

	bool start();
	virtual void notice_connection_open(int fd,int sn);
	virtual void notice_connection_close(int fd,int sn);

	PacketHandlerResult default_handler(ServerPacket* packet);
	PacketHandlerResult connection_open_handler(ServerPacket* packet);
	PacketHandlerResult connection_close_handler(ServerPacket* packet);
	PacketHandlerResult client_login_handler(ServerPacket* packet);
	PacketHandlerResult client_request_notice_handler(ServerPacket* packet);
	PacketHandlerResult client_request_policy_handler(ServerPacket* packet);
	PacketHandlerResult client_add_notice_handler(ServerPacket* packet);
	PacketHandlerResult client_add_policy_handler(ServerPacket* packet);
	PacketHandlerResult client_del_notice_handler(ServerPacket* packet);
	PacketHandlerResult client_del_policy_handler(ServerPacket* packet);
	PacketHandlerResult client_upload_handler(ServerPacket* packet);
	PacketHandlerResult client_request_traininfo_handler(ServerPacket* packet);
	PacketHandlerResult client_request_trainlist_handler(ServerPacket* packet);
	PacketHandlerResult client_modify_train_handler(ServerPacket* packet);
	PacketHandlerResult client_add_train_handler(ServerPacket* packet);
	PacketHandlerResult client_del_train_handler(ServerPacket* packet);
	PacketHandlerResult client_request_user_handler(ServerPacket* packet);

	void print();

private:
	UserInfoPtr get_user(const int sn);
	void send_packet(UserInfoPtr user, uint16 code, uint16 err);
	void send_packet(UserInfoPtr user, ServerPacket *packet);

private:
	uint32 _conns;
	map<string, UserInfoPtr> _users_name_map;
	map<int32, UserInfoPtr> _users_num_map;
	map<int32, UserInfoPtr> _users_sn_map;
	map<int32, TrainInfoPtr> _train_map;

	uint32 _send;
	uint32 _recv;
};

#endif /* SERVER_H_ */
#ifndef SERVER_H_
#define SERVER_H_

#include "common/serverbase/ServerBase.h"
#include "common/message/message_id.h"
#include "common/message/error_code.h"
#include "PlayerInfo.h"

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

	void print();

private:
	uint32 _conns;
	map<int32,PlayerInfoPtr> _players;

	uint32 _send;
	uint32 _recv;
};

#endif /* SERVER_H_ */
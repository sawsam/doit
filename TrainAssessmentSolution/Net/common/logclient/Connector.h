/*
 * Connector.h
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "common/serverbase/ServerBase.h"
#include "LogClientConfig.h"

#define MAX_STAND_BY_COUNT 512

/*
 * connector 负责 logclient 与 logserver 通讯
 */
class Connector : public common::ServerBase
{
private:
	bool _running;
	bool _connected;

	int32 _fd;
	int32 _sn;

	LogClientConfig _config;
	deque<ServerPacket *> _stand_by; //待发送的包

private:
	void connect();

	void send_stand_by();

	void append_packet(ServerPacket * packet);

public:
	Connector(const LogClientConfig & config);

	virtual ~Connector();

public:
	bool start();

	bool stop();

	bool is_running();

	bool is_connected(){return _connected;}

	bool send_packet(ServerPacket * packet);

public:
	PacketHandlerResult on_default(ServerPacket * packet);

	PacketHandlerResult on_connection_open(ServerPacket * packet);

	PacketHandlerResult on_connection_close(ServerPacket * packet);

	PacketHandlerResult on_module_login(ServerPacket * packet);

	PacketHandlerResult on_module_logout(ServerPacket * packet);
};

#endif /* CONNECTOR_H_ */


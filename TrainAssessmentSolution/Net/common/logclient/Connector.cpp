/*
 * Connector.cpp
 *
 *  Created on: 2013-5-10
 *      Author: dq
 */

#include "Connector.h"
#include "util/func_template.h"
#include "common/message/message_id.h"
#include "common/shared/datatype_define.h"

using namespace common;

Connector::Connector(const LogClientConfig & config): ServerBase(config),
	_running(false), _connected(false), _fd(-1), _sn(0), _config(config)
{
	insert_default_handle(&Connector::on_default);
	insert_handle(MCI_CONNECTION_OPEN, &Connector::on_connection_open);
	insert_handle(MCI_CONNECTION_CLOSE, &Connector::on_connection_close);
	insert_handle(MCI_MODULE_LOGIN, &Connector::on_module_login);
	insert_handle(MCI_MODULE_LOGOUT, &Connector::on_module_logout);

}

Connector::~Connector()
{
	sdk::deque_clean_and_free<ServerPacket *>(_stand_by);
}

void Connector::connect()
{
	ConnectInfo conn_info;
	conn_info._addr = _config._ip;
	conn_info._port = sdk::to_string(_config._port);
	conn_info._sn = _config._client_flag;

	_comm->request_connect_server(conn_info);
}

void Connector::send_stand_by()
{
	if(_running == false || _connected == false)
	{
		return;
	}

	while(_stand_by.size() > 0)
	{
		ServerPacket * packet = _stand_by.front();
		_stand_by.pop_front();

		if(packet == NULL)
		{
			continue;
		}

		packet->set_fd(_fd);
		packet->set_sn(_sn);

		_frame_send->push_packet(packet);
	}
}

void Connector::append_packet(ServerPacket * packet)
{
	if(_stand_by.size() >= MAX_STAND_BY_COUNT)
	{
		delete packet;

		return;
	}

	_stand_by.push_back(packet);
}


bool Connector::start()
{
	if(ServerBase::start() == false)
	{
		return false;
	}

	connect();

	_running = true;

	return true;
}

bool Connector::stop()
{
	_running = false;

	if(ServerBase::stop() == false)
	{
		return false;
	}

	return true;
}

bool Connector::is_running()
{
	return _running;
}

bool Connector::send_packet(ServerPacket * packet)
{
	push_packet(packet);

	return true;
}
PacketHandlerResult Connector::on_default(ServerPacket * packet)
{
	if(_running == false || _connected == false)
	{
		append_packet(packet);

		return REUSE_PACKET;
	}

	send_stand_by();

	packet->set_fd(this->_fd);
	packet->set_sn(this->_sn);

	_frame_send->push_packet(packet);

	return REUSE_PACKET;
}

PacketHandlerResult Connector::on_connection_open(ServerPacket * packet)
{
	_fd = packet->get_fd();
	_sn = packet->get_sn();

	ServerPacket * login_packet = new ServerPacket(_fd, _sn, NULL, 0);
	login_packet->set_code(MCI_MODULE_LOGIN);
	login_packet->set_pid(LOGGER);

	_frame_send->add_fd_sn(_fd, _sn);
	_frame_send->push_packet(login_packet);

	return DELETE_PACKET;
}

PacketHandlerResult Connector::on_connection_close(ServerPacket * packet)
{
	int32 fd = packet->get_fd();
	int32 sn = packet->get_sn();

	_connected = false;

	if(_frame_send)
	{
		_frame_send->del_fd_sn(fd, sn);
	}

	if(_running)
	{
		connect();
	}

	return DELETE_PACKET;
}

PacketHandlerResult Connector::on_module_login(ServerPacket * packet)
{
	switch(packet->get_pid())
	{
	case LOGGER:
	{
		_connected = true;

		send_stand_by();

	}
		break;
	default:
	{

	}
		break;
	}

	return DELETE_PACKET;
}

PacketHandlerResult Connector::on_module_logout(ServerPacket * packet)
{
	switch(packet->get_pid())
	{
	case LOGGER:
	{
		_connected = false;
	}
		break;
	default:
	{

	}
		break;
	}

	return DELETE_PACKET;
}



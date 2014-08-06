#include "Server.h"
#include "MyMessage.h"

Server::PrintTimer::PrintTimer(Server* server)
:_server(server)
{
}
Server::PrintTimer::~PrintTimer()
{
}

TimerHandleResult Server::PrintTimer::on_timer()
{
	_server->print();
	return THR_NORMAL;
}

Server::Server(const ServerBaseConfig& config)
:ServerBase(config),_conns(0),_send(0),_recv(0)
{
	insert_default_handle(&Server::default_handler);
	insert_handle(MCI_CONNECTION_OPEN,&Server::connection_open_handler);
	insert_handle(MCI_CONNECTION_CLOSE,&Server::connection_close_handler);
	insert_handle(MCC_LOGIN,&Server::client_login_handler);
	insert_handle(MCC_REQUEST_NOTICE,&Server::client_request_notice_handler);
	insert_handle(MCC_ADD_NOTICE,&Server::client_add_notice_handler);
	insert_handle(MCC_DEL_NOTICE,&Server::client_del_notice_handler);
	insert_handle(MCC_REQUEST_POLICY,&Server::client_request_policy_handler);
	insert_handle(MCC_ADD_POLICY,&Server::client_add_policy_handler);
	insert_handle(MCC_DEL_POLICY,&Server::client_del_policy_handler);
}

Server::~Server() 
{
}

bool Server::start()
{
	if (ServerBase::start() == false)
	{
		return false;
	}

	TimerHandlerPtr timer(new PrintTimer(this));
	add_timer(1000,true,timer);
	return true;
}

void Server::print()
{
	static int32 count = 0;
	int64 standy_num = get_standby_num();
	if (standy_num > 100000)
	{
		ofstream f;
		f.open("pressure_test.log",ios::out | ios::app);
		if (f.is_open())
		{
			boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
			string s = boost::posix_time::to_simple_string(t);
			stringstream ss;
			ss << "[" << s << "]" << "[   Server] conns: " << _conns << " standby num: " << standy_num << endl;
			ss << "[" << s << "]" << "[   Server] send: " << _send << " recv: " << _recv << endl;
			f << ss.str();
			f.close();
		}
	}
	if (++count == 10)
	{
		cout << "[   Server] conns: " << _conns << " standby num: " << standy_num << endl;
		cout << "[   Server] send: " << _send << " recv: " << _recv << endl;
		count = 0;
		_send = _recv = 0;
	}
}

void Server::notice_connection_open(int fd,int sn)
{
	ServerPacket* packet = new ServerPacket();
	packet->set_code(MCI_CONNECTION_OPEN);
	packet->set_fd(fd);
	packet->set_sn(sn);
	push_packet(packet);
}

void Server::notice_connection_close(int fd,int sn)
{
	ServerPacket* packet = new ServerPacket();
	packet->set_code(MCI_CONNECTION_CLOSE);
	packet->set_fd(fd);
	packet->set_sn(sn);
	push_packet(packet);
}

PacketHandlerResult Server::default_handler(ServerPacket* packet)
{
	cout << "[default_handler] code:" << packet->get_code() << endl;

	return DELETE_PACKET;
}

PacketHandlerResult Server::connection_open_handler(ServerPacket* packet)
{
	_frame_send->add_fd_sn(packet->get_fd(),packet->get_sn());
	++_conns;
	PlayerInfoPtr player(new PlayerInfo("", ""));
	player->_fd = packet->get_fd();
	player->_sn = packet->get_sn();
	_players.insert(make_pair(player->_sn,player));
	cout << "connection open fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;
	cout << "[connection]: " << _conns << endl;
	return DELETE_PACKET;
}

PacketHandlerResult Server::connection_close_handler(ServerPacket* packet)
{
	_frame_send->del_fd_sn(packet->get_fd(),packet->get_sn());
	--_conns;
	_players.erase(packet->get_sn());
	cout << "connection close fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;
	cout << "[connection]: " << _conns << endl;
	return DELETE_PACKET;
}

PacketHandlerResult Server::client_login_handler(ServerPacket* packet)
{
	cout << "client login fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;
	ServerPacket *p = new ServerPacket();
	p->set_fd(packet->get_fd());
	p->set_sn(packet->get_sn());
	p->set_code(MCS_LOGIN);
	send_packet(p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_notice_handler(ServerPacket* packet)
{
	static const int num_per_page = 10;

	StreamReader r(packet->get_body());
	int page = r.read_int32();
	if (page <= 0)
	{
		page = 1;
	}
	int start_index = (page - 1) * num_per_page;
	int end_index = page * num_per_page;
	list<NoticePtr> &notice_list = MyMessage::instance()->notice();
	int index = 0;
	int total_num = MyMessage::instance()->notice_count();
	int total_page = total_num % num_per_page ? total_num / num_per_page + 1 : total_num / num_per_page;
	if (page > total_page)
	{
		page = total_page;
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_REQUEST_NOTICE);
	p->set_fd(packet->get_fd());
	p->set_sn(packet->get_sn());
	StreamWriter w(p->get_body());
	w.write_int32(page);
	w.write_int32(total_page);
	int pos = w.position();
	w.write_int32(0);
	int count = 0;

	list<NoticePtr>::iterator iter = notice_list.begin();
	for (; iter != notice_list.end(); ++iter)
	{
		NoticePtr notice = *iter;
		if (notice->_complete)
		{
			++index;
			if (index <= start_index)
			{
				continue;
			}

			if (index > end_index)
			{
				break;
			}

			notice->serialize(w);
			++count;
		}
	}
	if (count > 0)
	{
		w.position() = pos;
		w.write_int32(count);
	}
	send_packet(p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_policy_handler(ServerPacket* packet)
{
	static const int num_per_page = 10;

	StreamReader r(packet->get_body());
	int page = r.read_int32();
	if (page <= 0)
	{
		page = 1;
	}
	int start_index = (page - 1) * num_per_page;
	int end_index = page * num_per_page;
	list<NoticePtr> &notice_list = MyMessage::instance()->policy();
	int index = 0;
	int total_num = MyMessage::instance()->policy_count();
	int total_page = total_num % num_per_page ? total_num / num_per_page + 1 : total_num / num_per_page;
	if (page > total_page)
	{
		page = total_page;
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_REQUEST_POLICY);
	p->set_fd(packet->get_fd());
	p->set_sn(packet->get_sn());
	StreamWriter w(p->get_body());
	w.write_int32(page);
	w.write_int32(total_page);
	int pos = w.position();
	w.write_int32(0);
	int count = 0;

	list<NoticePtr>::iterator iter = notice_list.begin();
	for (; iter != notice_list.end(); ++iter)
	{
		NoticePtr notice = *iter;
		if (notice->_complete)
		{
			++index;
			if (index <= start_index)
			{
				continue;
			}

			if (index > end_index)
			{
				break;
			}

			notice->serialize(w);
			++count;
		}
	}
	if (count > 0)
	{
		w.position() = pos;
		w.write_int32(count);
	}
	send_packet(p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_add_notice_handler(ServerPacket* packet)
{
	StreamReader r(packet->get_body());
	NoticePtr notice(new Notice());
	notice->unserialize(r);
	notice->_tick = time(0);
	if (notice->_attachments.size() == 0)
	{
		notice->_complete = true;
	}
	MyMessage::instance()->add_notice(notice);

	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ADD_NOTICE);
		p->set_fd(packet->get_fd());
		p->set_sn(packet->get_sn());
		StreamWriter w(p->get_body());
		w.write_int32(notice->_id);
		send_packet(p);
	}
	
	if (notice->_complete)
	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ACTIVE_ADD_NOTICE);
		p->set_fd(packet->get_fd());
		p->set_sn(packet->get_sn());
		send_packet(p);
	}

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_add_policy_handler(ServerPacket* packet)
{
	StreamReader r(packet->get_body());
	NoticePtr notice(new Notice());
	notice->unserialize(r);
	notice->_tick = time(0);
	if (notice->_attachments.size() == 0)
	{
		notice->_complete = true;
	}
	MyMessage::instance()->add_policy(notice);

	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ADD_POLICY);
		p->set_fd(packet->get_fd());
		p->set_sn(packet->get_sn());
		StreamWriter w(p->get_body());
		w.write_int32(notice->_id);
		send_packet(p);
	}
	
	if (notice->_complete)
	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ACTIVE_ADD_POLICY);
		p->set_fd(packet->get_fd());
		p->set_sn(packet->get_sn());
		send_packet(p);
	}

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_del_notice_handler(ServerPacket* packet)
{
	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		MyMessage::instance()->del_notice(r.read_int32());
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_ACTIVE_DEL_NOTICE);
	p->set_fd(packet->get_fd());
	p->set_sn(packet->get_sn());
	send_packet(p);

	return DELETE_PACKET;

}

PacketHandlerResult Server::client_del_policy_handler(ServerPacket* packet)
{
	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		MyMessage::instance()->del_policy(r.read_int32());
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_ACTIVE_DEL_POLICY);
	p->set_fd(packet->get_fd());
	p->set_sn(packet->get_sn());
	send_packet(p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_upload_handler(ServerPacket* packet)
{
	StreamReader r(packet->get_body());
	int id = r.read_int32();
	int type = r.read_int32();
	string filename = r.read_string();
	if (type == 0)
	{
		list<NoticePtr> &notice_list = MyMessage::instance()->notice();
		list<NoticePtr>::iterator iter = notice_list.begin();
		for (; iter != notice_list.end(); ++iter)
		{
			NoticePtr notice = *iter;
			if (notice->_id == id)
			{
				break;
			}
		}
		if (iter == notice_list.end())
		{
			send_packet(packet->get_fd(), packet->get_sn(), packet->get_uid(), packet->get_pid(), MCS_UPLOAD_ATTACHMENT, 0);
			return DELETE_PACKET;
		}
		NoticePtr notice = *iter;
		
	}
	else
	{
	}
}
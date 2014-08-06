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
	{
		UserInfoPtr user(new UserInfo("admin","admin",1,"管理员","心血管内科"));
		this->_users_name_map.insert(std::make_pair(user->_user_name, user));
		this->_users_num_map.insert(std::make_pair(user->_number, user));
	}

	{
		UserInfoPtr user(new UserInfo("admin1","admin",2,"管理员1","心血管内科"));
		this->_users_name_map.insert(std::make_pair(user->_user_name, user));
		this->_users_num_map.insert(std::make_pair(user->_number, user));
	}

	{
		UserInfoPtr user(new UserInfo("admin2","admin",3,"管理员2","心血管内科"));
		this->_users_name_map.insert(std::make_pair(user->_user_name, user));
		this->_users_num_map.insert(std::make_pair(user->_number, user));
	}

	{
		UserInfoPtr user(new UserInfo("admin3","admin",4,"管理员3","心血管内科"));
		this->_users_name_map.insert(std::make_pair(user->_user_name, user));
		this->_users_num_map.insert(std::make_pair(user->_number, user));
	}

	{
		UserInfoPtr user(new UserInfo("admin4","admin",5,"管理员4","心血管内科"));
		this->_users_name_map.insert(std::make_pair(user->_user_name, user));
		this->_users_num_map.insert(std::make_pair(user->_number, user));
	}

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
	insert_handle(MCC_REQUEST_TRAIN_INFO,&Server::client_request_traininfo_handler);
	insert_handle(MCC_REQUEST_TRAIN_LIST,&Server::client_request_trainlist_handler);
	insert_handle(MCC_MODIFY_TRAIN,&Server::client_modify_train_handler);
	insert_handle(MCC_ADD_TRAIN,&Server::client_add_train_handler);
	insert_handle(MCC_DEL_TRAIN,&Server::client_del_train_handler);
	insert_handle(MCC_REQUEST_USER,&Server::client_request_user_handler);
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

UserInfoPtr Server::get_user(const int sn)
{
	map<int, UserInfoPtr>::iterator iter = this->_users_sn_map.find(sn);
	if (iter != this->_users_sn_map.end())
	{
		return iter->second;
	}
	return UserInfoPtr();
}

void Server::send_packet(UserInfoPtr user, uint16 code, uint16 err)
{
	ServerBase::send_packet(user->_fd, user->_sn, 0, 0, code, err);
}

void Server::send_packet(UserInfoPtr user, ServerPacket *packet)
{
	packet->set_fd(user->_fd);
	packet->set_sn(user->_sn);
	ServerBase::send_packet(packet);
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
	cout << "connection open fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;
	cout << "[connection]: " << _conns << endl;
	return DELETE_PACKET;
}

PacketHandlerResult Server::connection_close_handler(ServerPacket* packet)
{
	_frame_send->del_fd_sn(packet->get_fd(),packet->get_sn());
	--_conns;
	this->_users_sn_map.erase(packet->get_sn());
	cout << "connection close fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;
	cout << "[connection]: " << _conns << endl;
	return DELETE_PACKET;
}

PacketHandlerResult Server::client_login_handler(ServerPacket* packet)
{
	cout << "client login fd: " << packet->get_fd() << " sn: " << packet->get_sn() << endl;

	StreamReader r(packet->get_body());
	string user_name = r.read_string();
	string passwd = r.read_string();
	map<string, UserInfoPtr>::iterator iter = this->_users_name_map.find(user_name);
	if (iter == this->_users_name_map.end())
	{
		ServerBase::send_packet(packet->get_fd(), packet->get_sn(), packet->get_uid(), packet->get_pid(), MCS_LOGIN, E_PASSWORD_ERR);
		return DELETE_PACKET;
	}
	UserInfoPtr user = iter->second;
	if (user->_passwd != passwd)
	{
		ServerBase::send_packet(packet->get_fd(), packet->get_sn(), packet->get_uid(), packet->get_pid(), MCS_LOGIN, E_PASSWORD_ERR);
		return DELETE_PACKET;
	}
	user->_fd = packet->get_fd();
	user->_sn = packet->get_sn();
	this->_users_sn_map.insert(std::make_pair(user->_sn, user));

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_LOGIN);
	StreamWriter w(p->get_body());
	w.write_int32(user->_number);
	w.write_string(user->_name);
	w.write_string(user->_department);
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_notice_handler(ServerPacket* packet)
{
	static const int num_per_page = 10;

	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

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
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_policy_handler(ServerPacket* packet)
{
	static const int num_per_page = 10;

	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

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
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_add_notice_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

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
		StreamWriter w(p->get_body());
		w.write_int32(notice->_id);
		send_packet(user, p);
	}
	
	if (notice->_complete)
	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ACTIVE_ADD_NOTICE);
		send_packet(user, p);
	}

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_add_policy_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

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
		StreamWriter w(p->get_body());
		w.write_int32(notice->_id);
		send_packet(user, p);
	}
	
	if (notice->_complete)
	{
		ServerPacket *p = new ServerPacket();
		p->set_code(MCS_ACTIVE_ADD_POLICY);
		send_packet(user, p);
	}

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_del_notice_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		MyMessage::instance()->del_notice(r.read_int32());
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_ACTIVE_DEL_NOTICE);
	send_packet(user, p);

	return DELETE_PACKET;

}

PacketHandlerResult Server::client_del_policy_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		MyMessage::instance()->del_policy(r.read_int32());
	}

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_ACTIVE_DEL_POLICY);
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_upload_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

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
			send_packet(user, MCS_UPLOAD_ATTACHMENT, E_NO_ERR);
			return DELETE_PACKET;
		}
		NoticePtr notice = *iter;
		
	}
	else
	{
	}

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_traininfo_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	map<int, TrainInfoPtr>::iterator iter = this->_train_map.find(user->_number);
	if (iter == this->_train_map.end())
	{
		return DELETE_PACKET;
	}

	TrainInfoPtr train = iter->second;
	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_REQUEST_TRAIN_INFO);
	StreamWriter w(p->get_body());
	w.write_int32(user->_number);
	w.write_string(user->_name);
	w.write_string(user->_department);
	w.write_string(train->_discipline);
	w.write_string(train->_stage);
	w.write_int32(train->_period);
	w.write_int32(train->_trains.size());
	for (size_t i = 0; i < train->_trains.size(); ++i)
	{
		train->_trains[i]->serialize(w);
	}
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_trainlist_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	string department = r.read_string();
	string name = r.read_string();
	int number = r.read_int32();

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_REQUEST_TRAIN_LIST);
	StreamWriter w(p->get_body());
	int count = 0;
	int pos = w.position();
	w.write_int32(count);
	map<int, TrainInfoPtr>::iterator iter = this->_train_map.begin();
	for (; iter != this->_train_map.end(); ++iter)
	{
		TrainInfoPtr train = iter->second;
		map<int, UserInfoPtr>::iterator user_iter = this->_users_num_map.find(train->_number);
		if (user_iter != this->_users_num_map.end())
		{
			UserInfoPtr user = user_iter->second;
			if ((user->_department == department || department == "") &&
				(user->_name == name || name == "") &&
				(user->_number == number || number == 0))
			{
				w.write_string(user->_department);
				w.write_int32(user->_number);
				w.write_string(user->_name);
				w.write_string(train->_stage);
				w.write_int32(train->_period);
				count++;
			}
		}
	}
	w.position() = pos;
	w.write_int32(count);
	send_packet(user, p);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_modify_train_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		int number = r.read_int32();
		string stage = r.read_string();
		int period = r.read_int32();
		map<int, TrainInfoPtr>::iterator iter = this->_train_map.find(number);
		if (iter == this->_train_map.end())
		{
			continue;
		}
		TrainInfoPtr train = iter->second;
		train->_stage = stage;
		train->_period = period;
	}

	send_packet(user, MCS_MODIFY_TRAIN, E_NO_ERR);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_add_train_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		int number = r.read_int32();
		string stage = r.read_string();
		int period = r.read_int32();
		map<int, UserInfoPtr>::iterator iter = this->_users_num_map.find(number);
		if (iter == this->_users_num_map.end())
		{
			continue;
		}
		UserInfoPtr user = iter->second;
		TrainInfoPtr train(new TrainInfo());
		train->_number = user->_number;
		train->_stage = stage;
		train->_period = period;
		this->_train_map.insert(std::make_pair(train->_number, train));
	}

	send_packet(user, MCS_ADD_TRAIN, E_NO_ERR);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_del_train_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	int count = r.read_int32();
	for (int i = 0; i < count; ++i)
	{
		int number = r.read_int32();
		this->_train_map.erase(number);
	}

	send_packet(user, MCS_DEL_TRAIN, E_NO_ERR);

	return DELETE_PACKET;
}

PacketHandlerResult Server::client_request_user_handler(ServerPacket* packet)
{
	UserInfoPtr user = this->get_user(packet->get_sn());
	if (!user)
	{
		return DELETE_PACKET;
	}

	StreamReader r(packet->get_body());
	string department = r.read_string();
	string name = r.read_string();
	int number = r.read_int32();

	ServerPacket *p = new ServerPacket();
	p->set_code(MCS_REQUEST_USER);
	StreamWriter w(p->get_body());
	int count = 0;
	int pos = w.position();
	w.write_int32(count);
	map<int, UserInfoPtr>::iterator iter = this->_users_num_map.begin();
	for (; iter != this->_users_num_map.end(); ++iter)
	{
		UserInfoPtr user = iter->second;
		if ((user->_department == department || department == "") &&
			(user->_name == name || name == "") &&
			(user->_number == number || number == 0))
		{
			w.write_string(user->_department);
			w.write_int32(user->_number);
			w.write_string(user->_name);
			count++;
		}
	}
	w.position() = pos;
	w.write_int32(count);
	send_packet(user, p);

	return DELETE_PACKET;
}
#include "CommModule.h"

namespace sdk
{
CommModule::CommModule(const CommConfig& config,CommInterface* owner)
:_config(config),_owner(owner),_work(_io_service),_sn(0)
{
}

CommModule::~CommModule()
{
}

bool CommModule::start()
{
	if (_config._enable_acceptor)
	{
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),_config._listen_port);
		_acceptor = boost::shared_ptr<boost::asio::ip::tcp::acceptor>(new boost::asio::ip::tcp::acceptor(_io_service,endpoint));
		start_accept();
	}

	return EventFrame::start();
}

bool CommModule::stop()
{
	_io_service.stop();

	return EventFrame::stop();
}

void CommModule::notice_connection_open(int fd,int sn)
{
	if (sn > 0)
	{
		std::hash_map<int,ConnectionPtr>::iterator iter = _connecting.find(sn);
		if (iter != _connecting.end())
		{
			ConnectionPtr conn = iter->second;
			_connecting.erase(iter);
			int64 key = sdk::MAKEINT64(fd,sn);
			_connected.insert(make_pair(key,conn));
		}
	}
	_owner->notice_connection_open(fd,sn);
}

void CommModule::notice_connection_close(int fd,int sn)
{
	int64 key = sdk::MAKEINT64(fd,sn);
	std::hash_map<int64,ConnectionPtr>::iterator iter = _connected.find(key);
	if (iter == _connected.end())
	{
		return;
	}
	_connected.erase(iter);
	_owner->notice_connection_close(fd,sn);
}

void CommModule::notice_message_arrive(int fd,int sn,char* msg,size_t len)
{
	_owner->notice_message_arrive(fd,sn,msg,len);
}

void CommModule::notice_connect_error(int sn)
{
	_connecting.erase(sn);
	_owner->notice_connect_error(sn);
}

void CommModule::request_connect_server(const ConnectInfo& ci)
{
	if (ci._addr.length() > 0 && ci._port.length() > 0 && ci._sn > 0)
	{
		_io_service.post(boost::bind(&CommModule::do_connect_server,this,ci));
	}
}

void CommModule::request_close_connection(int fd,int sn)
{//post保证操作在io_service.run()所在线程执行
	_io_service.post(boost::bind(&CommModule::do_close_connection,this,fd,sn));
}

void CommModule::request_send_message(int fd,int sn,const char* msg,size_t len)
{
	MsgSendPtr msg_send(new MsgSend(fd,sn,msg,len));
	_io_service.post(boost::bind(&CommModule::do_send_message,this,msg_send));
}

void CommModule::notice_message_send(int fd,int sn)
{
	_owner->notice_message_send(fd,sn);
}

void CommModule::do_connect_server(ConnectInfo ci)
{
	if (_connecting.count(ci._sn) > 0 )
	{
		return;
	}
	ConnectionPtr conn(new Connection(_config._conconfig,this,_io_service));
	_connecting.insert(make_pair(ci._sn,conn));
	conn->connect(ci);
}

void CommModule::do_close_connection(int fd,int sn)
{
	int64 key = sdk::MAKEINT64(fd,sn);
	std::hash_map<int64,ConnectionPtr>::iterator iter = _connected.find(key);
	if (iter == _connected.end())
	{
		return;
	}
	ConnectionPtr conn = iter->second;
	conn->stop();
}

void CommModule::do_send_message(MsgSendPtr msg)
{
	int64 key = sdk::MAKEINT64(msg->_fd,msg->_sn);
	std::hash_map<int64,ConnectionPtr>::iterator iter = _connected.find(key);
	if (iter == _connected.end())
	{
		return;
	}
	ConnectionPtr conn = iter->second;
	SendBuf* buf = new SendBuf(msg->_msg,msg->_len);
	conn->send(buf);
}

void CommModule::user_process()
{
	_io_service.run();
}

void CommModule::start_accept()
{
	ConnectionPtr conn(new Connection(_config._conconfig,this,_io_service));
	_acceptor->async_accept(conn->socket(),
		boost::bind(&CommModule::handle_accept,this,conn,
        boost::asio::placeholders::error));
}

void CommModule::handle_accept(ConnectionPtr conn,const boost::system::error_code& error)
{
	if (!error)
	{
		conn->set_fd(conn->socket().native_handle());
		conn->set_sn(get_sn());
		int64 key = sdk::MAKEINT64(conn->get_fd(),conn->get_sn());
		_connected.insert(make_pair(key,conn));
		conn->start();
	}
	start_accept();
}
}
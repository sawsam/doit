#include "Connection.h"
#include "util/func_template.h"

namespace sdk
{
Connection::Connection(ConnectionConfig& config,CommInterface* owner,boost::asio::io_service& io_service)
:_config(config),_owner(owner),_socket(io_service),_idle_timer(io_service),_connect_timer(io_service),
_reconnect_timer(io_service),_rbuf(_config._max_read_unit),_fd(0),_sn(0),_last_recv_count(0),
_last_recv_time(0),_send_count(0),_recv_count(0)
{
}

Connection::~Connection()
{
	queue_clean_and_free(_send_q);
}

void Connection::connect(const ConnectInfo& ci)
{
	_connect_timer.expires_from_now(boost::posix_time::milliseconds(ci._timeout));
	_connect_timer.async_wait(boost::bind(&Connection::handle_connect_timeout,shared_from_this(),ci,
		boost::asio::placeholders::error));

	boost::asio::ip::tcp::resolver resolver(_socket.get_io_service());
	boost::asio::ip::tcp::resolver::query query(ci._addr, ci._port);
	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
	boost::asio::async_connect(_socket, iterator,
		boost::bind(&Connection::handle_connect, shared_from_this(), ci,
        boost::asio::placeholders::error));
}

void Connection::start()
{
	_owner->notice_connection_open(get_fd(),get_sn());
	boost::asio::async_read(_socket,
		boost::asio::buffer(_rbuf.data(), sizeof(MsgHead)),
		boost::bind(&Connection::handle_read_header, shared_from_this(),
		boost::asio::placeholders::error));

	if (get_sn() > 0)
	{//设置心跳发送timer，sn > 0 表示连出连接，需要发送心跳包
		_idle_timer.expires_from_now(boost::posix_time::milliseconds(_config._idle_limit >> 1));
		_idle_timer.async_wait(boost::bind(&Connection::handle_heartbeat,shared_from_this(),boost::asio::placeholders::error));
	}
	else
	{//设置空闲检测timer，sn < 0 表示连入连接，需要空闲检测
		_idle_timer.expires_from_now(boost::posix_time::milliseconds(_config._idle_limit));
		_idle_timer.async_wait(boost::bind(&Connection::handle_idle,shared_from_this(),boost::asio::placeholders::error));
	}
}

void Connection::stop()
{
	_socket.close();
	_idle_timer.cancel();
	_connect_timer.cancel();
	_reconnect_timer.cancel();
	_owner->notice_connection_close(get_fd(),get_sn());
}

void Connection::send(SendBuf* msg)
{
	bool write_in_process = !_send_q.empty();
	_send_q.push(msg);
	if (!write_in_process)
	{
		boost::asio::async_write(_socket,
			boost::asio::buffer(_send_q.front()->data(),
            _send_q.front()->len()),
			boost::bind(&Connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
	}
}

void Connection::handle_read_header(const boost::system::error_code& error)
{
	if (!error)
    {
		if (_rbuf.body_len() > _config._max_read_unit)
		{//检测消息长度
			cout << "[Connection] close for unit fd: " << get_fd() << " sn: " << get_sn() << endl;
			stop();
			return;
		}

		boost::asio::async_read(_socket,
			boost::asio::buffer(_rbuf.body(), _rbuf.body_len()),
			boost::bind(&Connection::handle_read_body, shared_from_this(),
			boost::asio::placeholders::error));
    }
    else
    {
		stop();
    }
}

void Connection::handle_read_body(const boost::system::error_code& error)
{
	if (!error)
    {
		++_last_recv_count;
		++_recv_count;
		if (_last_recv_count > _config._max_read_rate)
		{//检测消息频率
			time_t now = time(0);
			if (now - _last_recv_time <= 1)
			{
				cout << "[Connection] close for rate fd: " << get_fd() << " sn: " << get_sn() << endl;
				stop();
				return;
			}
			_last_recv_count = 0;
			_last_recv_time = now;
		}
		if (_rbuf.body_len() > 0)
		{
			MsgHead* head = (MsgHead*)_rbuf.data();
			boost::crc_32_type result;
			result.process_bytes(_rbuf.body(),_rbuf.body_len());
			if (result.checksum() != head->_crc32)
			{//检测消息体校验值
				cout << "[Connection] close for checksum: " << get_fd() << " sn: " << get_sn() << endl;
				stop();
				return;
			}
			_owner->notice_message_arrive(get_fd(),get_sn(),_rbuf.body(),_rbuf.body_len());
		}
		else
		{
			//cout << "[Connection] fd: " << get_fd() << " sn: " << get_sn() << " receive a heartbeat" << endl;
		}

		boost::asio::async_read(_socket,
			boost::asio::buffer(_rbuf.data(), sizeof(MsgHead)),
			boost::bind(&Connection::handle_read_header, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
		stop();
    }
}

void Connection::handle_write(const boost::system::error_code& error)
{
	if (!error)
    {
		++_send_count;
		SendBuf* msg = _send_q.front();
		delete msg;
		_send_q.pop();
		if (!_send_q.empty())
		{
			  boost::asio::async_write(_socket,
				  boost::asio::buffer(_send_q.front()->data(),
				  _send_q.front()->len()),
				  boost::bind(&Connection::handle_write, shared_from_this(),
				  boost::asio::placeholders::error));
		}
		//通知上层一个消息已发送完成
		_owner->notice_message_send(get_fd(),get_sn());
    }
    else
    {
		stop();
    }
}

void Connection::handle_connect(ConnectInfo ci,const boost::system::error_code& error)
{
	if (_socket.is_open())
	{//保证连接超时回调函数还没被执行
		_connect_timer.expires_at(boost::posix_time::pos_infin);//设置连接超时为无限
		if (!error)
		{//连接成功则开启连接
			set_fd(_socket.native_handle());
			set_sn(ci._sn);
			start();
		}
		else
		{//连接失败则设置连接timer
			cout << "connect server: " << ci._addr << ":" << ci._port << " sn: " << ci._sn << " error" << endl;
			if (ci._reconnect)
			{
				_reconnect_timer.expires_from_now(boost::posix_time::milliseconds(_config._reconnect_interval));
				_reconnect_timer.async_wait(boost::bind(&Connection::handle_reconect_timeout,shared_from_this(),ci,boost::asio::placeholders::error));
			}
			else
			{
				_owner->notice_connect_error(ci._sn);
			}
		}
	}
}

void Connection::handle_connect_timeout(ConnectInfo ci,const boost::system::error_code& error)
{
	if (_connect_timer.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{//检查在此方法被执行之前timer超时时间没有被重设
		_socket.close();

		cout << "connect server: " << ci._addr << ":" << ci._port << " sn: " << ci._sn << " timeout" << endl;
		if (ci._reconnect)
		{
			_reconnect_timer.expires_from_now(boost::posix_time::milliseconds(_config._reconnect_interval));
			_reconnect_timer.async_wait(boost::bind(&Connection::handle_reconect_timeout,shared_from_this(),ci,boost::asio::placeholders::error));
		}
		else
		{
			_owner->notice_connect_error(ci._sn);
		}
	}
}

void Connection::handle_reconect_timeout(ConnectInfo ci,const boost::system::error_code& error)
{
	if (!error)
	{//重新连接
		connect(ci);
	}
}

void Connection::handle_idle(const boost::system::error_code& error)
{
	if (!error)
	{//空闲检测
		//cout << "[Connection] idle check fd: " << get_fd() << " sn: " << get_sn() << endl;
		if (_send_count + _recv_count == 0)
		{
			cout << "[Connection] close for idle fd: " << get_fd() << " sn: " << get_sn() << endl;
			stop();
			return;
		}
		_send_count = _recv_count = 0;
		_idle_timer.expires_at(_idle_timer.expires_at() + boost::posix_time::milliseconds(_config._idle_limit));
		_idle_timer.async_wait(boost::bind(&Connection::handle_idle,shared_from_this(),boost::asio::placeholders::error));
	}
}

void Connection::handle_heartbeat(const boost::system::error_code& error)
{
	if (!error)
	{//发送心跳包
		SendBuf* msg = new SendBuf();
		send(msg);
		//cout << "send heartbeat fd: " << get_fd() << " sn: " << get_sn() << endl;
		_idle_timer.expires_at(_idle_timer.expires_at() + boost::posix_time::milliseconds(_config._idle_limit >> 1));
		_idle_timer.async_wait(boost::bind(&Connection::handle_heartbeat,shared_from_this(),boost::asio::placeholders::error));
	}
}
}
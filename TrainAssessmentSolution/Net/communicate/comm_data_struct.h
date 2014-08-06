#ifndef COMM_DATA_STRUCT_H_
#define COMM_DATA_STRUCT_H_

#include "global/base_inc.h"

namespace sdk
{
enum CHSWITCH_FLAG
{
	CHSF_ZIP = 0x01
};

//--2个字节
struct CHSwitcher
{
	CHSwitcher()
	:_b1(0), _b2(0)
	{}
	CHSwitcher(const CHSwitcher & r)
	:_b1(r._b1), _b2(r._b2)
	{}
	~CHSwitcher(){}

	inline void set_zip(){_b1 &= CHSF_ZIP;}
	inline bool is_zip(){return (_b1 & CHSF_ZIP) ? true : false;}

	unsigned char _b1;
	unsigned char _b2;
};

//发送消息头
struct MsgHead
{
	unsigned short _len;//消息总长度
	CHSwitcher _switch16;
	unsigned int _crc32;//消息体的crc校验值
	union
	{
		struct
		{
			int _fd;
			int _sn;
		}_client_info;
		unsigned char _c64bit[8];
	}_info64;
};

//发送消息命令
struct MsgSend
{
	MsgSend(int fd,int sn,const char* msg,size_t len)
	:_fd(fd),_sn(sn),_msg(new char[len]),_len(len)
	{
		memcpy(_msg,msg,len);
	}
	~MsgSend()
	{
		delete []_msg;
	}

	int _fd;
	int _sn;
	char* _msg;
	size_t _len;
};
typedef boost::shared_ptr<MsgSend> MsgSendPtr;

//接收缓冲
struct ReceiveBuf
{
	ReceiveBuf(size_t max_read_unit)
	:_buf(new char[sizeof(MsgHead) + max_read_unit])
	{
	}

	~ReceiveBuf()
	{
		delete []_buf;
	}

	inline char* data()
	{
		return &_buf[0];
	}

	inline size_t len()
	{
		MsgHead* head = (MsgHead*)&_buf[0];
		return head->_len;
	}

	inline char* body()
	{
		return &_buf[sizeof(MsgHead)];
	}

	inline size_t body_len()
	{
		MsgHead* head = (MsgHead*)&_buf[0];
		return head->_len - sizeof(MsgHead);
	}

	char* _buf;
};

//发送缓冲
struct SendBuf
{
	SendBuf()
	:_len(sizeof(MsgHead)),_buf(new char[sizeof(MsgHead)])
	{
		MsgHead* head = (MsgHead*)&_buf[0];
		head->_len = _len;
		head->_crc32 = 0;
	}

	SendBuf(char* msg,size_t len)
	:_len(len + sizeof(MsgHead)),_buf(new char[len + sizeof(MsgHead)])
	{
		MsgHead* head = (MsgHead*)&_buf[0];
		head->_len = _len;
		boost::crc_32_type result;
		result.process_bytes(msg,len);
		head->_crc32 = result.checksum();
		memcpy(&_buf[sizeof(MsgHead)],msg,len);
	}

	~SendBuf()
	{
		delete []_buf;
	}

	inline char* data()
	{
		return &_buf[0];
	}

	inline size_t len()
	{
		return _len;
	}

	size_t _len;
	char* _buf;
};

//连接信息
#define DEF_CONNECT_TIMEOUT 5000
struct ConnectInfo
{
	ConnectInfo()
	:_addr(""),_port(""),_sn(0),_timeout(DEF_CONNECT_TIMEOUT),_reconnect(true)
	{
	}

	string _addr;
	string _port;
	int _sn;
	int _timeout;//连接超时时间ms
	bool _reconnect;//连接不上时是否重连
};
}

#endif /* COMM_DATA_STRUCT_H_ */
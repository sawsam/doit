// ClientNet.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ClientNet.h"

extern "C"
{
__declspec(dllexport) void EnableAcceptor()
{
	ClientNet::instance()->enable_acceptor();
}

__declspec(dllexport) void SetListenPort(int port)
{
	ClientNet::instance()->set_listen_port(port);
}

__declspec(dllexport) void SetMaxReadUnit(int mru)
{
	ClientNet::instance()->set_max_read_unit(mru);
}

__declspec(dllexport) void ConnectServer(char* addr,char* port,int sn,int timeout,bool reconnect)
{
	ConnectInfo ci;
	ci._addr = addr;
	ci._port = port;
	ci._sn = sn;
	ci._timeout = timeout;
	ci._reconnect = reconnect;
	ClientNet::instance()->request_connect_server(ci);
}

__declspec(dllexport) void CloseConnection(int fd,int sn)
{
	ClientNet::instance()->request_close_connection(fd,sn);
}

__declspec(dllexport) BOOL Start()
{
	return ClientNet::instance()->start();
}

__declspec(dllexport) BOOL Stop()
{
	return ClientNet::instance()->stop();
}

__declspec(dllexport) void PacketReset()
{
	ClientNet::instance()->packet_reset();
}

__declspec(dllexport) BOOL PickPacket(int& fd,int& sn,int& code,int& err)
{
	int64 uid = 0;
	int pid = 0;
	return ClientNet::instance()->pick(fd,sn,uid,pid,code,err);
}

__declspec(dllexport) void SendPacket()
{
	ClientNet::instance()->send_packet();
}

__declspec(dllexport) char ReadByte()
{
	StreamReader* reader = StreamRW::instance()->get_reader();
	if (reader)
	{
		return reader->read_int8();
	}
	return 0;
}

__declspec(dllexport) int ReadInt()
{
	StreamReader* reader = StreamRW::instance()->get_reader();
	if (reader)
	{
		return reader->read_int32();
	}
	return 0;
}

__declspec(dllexport) float ReadFloat()
{
	StreamReader* reader = StreamRW::instance()->get_reader();
	if (reader)
	{
		return reader->read_float();
	}
	return 0;
}

__declspec(dllexport) char* ReadString()
{
	static char s[MAX_STRING_LEN];
	memset(s,0,sizeof(s));
	StreamReader* reader = StreamRW::instance()->get_reader();
	if (reader)
	{
		string ss = reader->read_string();
		int len = ss.length() > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : ss.length();
		memcpy(s,ss.c_str(),len);
	}
	return s;
}

__declspec(dllexport) int ReadBytes(char* value, int size)
{
	StreamReader* reader = StreamRW::instance()->get_reader();
	if (reader)
	{
		return reader->read_bytes(value, size);
	}
	return 0;
}

__declspec(dllexport) void CreateSendPacket(int fd,int sn,int code,int err)
{
	StreamRW::instance()->set_write_packet();
	ServerPacket* packet = StreamRW::instance()->get_write_packet();
	packet->set_fd(fd);
	packet->set_sn(sn);
	packet->set_uid(0);
	packet->set_pid(0);
	packet->set_code(code);
	packet->set_error(err);
}

__declspec(dllexport) void WriteByte(char value)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->write_uint8(value);
	}
}

__declspec(dllexport) void WriteInt(int value)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->write_int32(value);
	}
}

__declspec(dllexport) void WriteFloat(float value)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->write_float(value);
	}
}

__declspec(dllexport) void WriteString(char* value)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->write_string(value);
	}
}

__declspec(dllexport) void WriteBytes(char* value, int size)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->write_bytes(value, size);
	}
}

__declspec(dllexport) int GetWritePos()
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		return writer->position();
	}
	return -1;
}

__declspec(dllexport) void SetWritePos(int pos)
{
	StreamWriter* writer = StreamRW::instance()->get_writer();
	if (writer)
	{
		writer->position() = pos;
	}
}
}


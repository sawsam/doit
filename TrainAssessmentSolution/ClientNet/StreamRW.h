#ifndef STREAMRW_H_
#define STREAMRW_H_

#include "stdafx.h"

#include "tools/ServerPacket.h"
#include "tools/StreamReader.h"
#include "tools/StreamWriter.h"
#include <string>

using namespace std;
using namespace sdk;

#define MAX_STRING_LEN 4096

class StreamRW
{
public:
	static StreamRW* instance()
	{
		static StreamRW _stream_rw;
		return &_stream_rw;
	}

	void set_read_packet(ServerPacket* packet)
	{
		if (!packet)
		{
			return;
		}

		release_read_packet();
		_read_packet = packet;
		_r = new StreamReader(_read_packet->get_body());
	}

	ServerPacket* get_read_packet()
	{
		return _read_packet;
	}

	void set_write_packet()
	{
		release_write_packet();
		_write_packet = new ServerPacket();
		_w = new StreamWriter(_write_packet->get_body());
	}

	ServerPacket* get_write_packet()
	{
		return _write_packet;
	} 

	StreamReader* get_reader()
	{
		return _r;
	}

	StreamWriter* get_writer()
	{
		return _w;
	}

	void reset_write_packet()
	{
		_write_packet = NULL;
		_w = NULL;
	}

	void release_read_packet()
	{
		if (_read_packet)
		{
			delete _read_packet;
			_read_packet = NULL;
		}
		if (_r)
		{
			delete _r;
			_r = NULL;
		}
	}

	void release_write_packet()
	{
		if (_write_packet)
		{
			delete _write_packet;
			_write_packet = NULL;
		}
		if (_w)
		{
			delete _w;
			_w = NULL;
		}
	}

private:
	StreamRW():_read_packet(NULL),_write_packet(NULL),_r(NULL),_w(NULL) {}
	~StreamRW() {}

private:
	ServerPacket* _read_packet;
	ServerPacket* _write_packet;
	StreamReader* _r;
	StreamWriter* _w;
};

#endif /* STREAMRW_H_ */
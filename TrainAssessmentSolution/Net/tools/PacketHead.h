#ifndef PACKETHEAD_H
#define PACKETHEAD_H

namespace sdk
{
#define MBL_64_BYTE		64			//小型消息体
#define MBL_256_BYTE	256			//小中型消息体
#define MBL_512_BYTE	512			//中型消息体
#define MBL_1536_BYTE	1536		//中大型消息体
#define MBL_3072_BYTE	3072		//大型消息体
#define MBL_10240_BYTE	10240		//大大型消息体

struct CSPacketHead	//不要用来做基类
{
	CSPacketHead()
	: _id(0), _ec(0)
	{}
	CSPacketHead(const CSPacketHead & r)
	: _id(r._id), _ec(r._ec)
	{}
	~CSPacketHead(){}

	uint16 _id;	//message id
	uint16 _ec;	//error code
};

struct SSPacketHead	//不要用来做基类
{
	SSPacketHead()
	: _uid(0),_pid(0),_id(0), _ec(0)
	{}
	SSPacketHead(const SSPacketHead & r)
	: _uid(r._uid),_pid(r._pid),_id(r._id), _ec(r._ec)
	{}
	~SSPacketHead(){}

	uint64 _uid; //unique id (example: MAKEINT64(fd, sn))
	int32 _pid;	//player id or module id
	uint16 _id;	//message id
	uint16 _ec;	//error code
};
}

#endif /* PACKETHEAD_H */
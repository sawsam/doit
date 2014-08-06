#ifndef PACKETHEAD_H
#define PACKETHEAD_H

namespace sdk
{
#define MBL_64_BYTE		64			//С����Ϣ��
#define MBL_256_BYTE	256			//С������Ϣ��
#define MBL_512_BYTE	512			//������Ϣ��
#define MBL_1536_BYTE	1536		//�д�����Ϣ��
#define MBL_3072_BYTE	3072		//������Ϣ��
#define MBL_10240_BYTE	10240		//�������Ϣ��

struct CSPacketHead	//��Ҫ����������
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

struct SSPacketHead	//��Ҫ����������
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
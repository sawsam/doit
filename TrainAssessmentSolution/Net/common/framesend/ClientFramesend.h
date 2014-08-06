/*
 * ClientFramesend.h
 *
 *  Created on: Oct 19, 2010
 *      Author: work
 */

#ifndef CLIENTFRAMESEND_H_
#define CLIENTFRAMESEND_H_

#include "FramesendBase.h"
#include "tools/ServerPacket.h"
#include "tools/ClientPacket.h"

/*
 * ClientFramesend ���ڷ�֡���͸�Client
 *
 * ʹ�÷�����
 * 1 ���ã�ͨ��FramesendBaseConfig����_timeout _bufsize
 * 2 �����߼�����ֱ��ʹ�ã�
 * ����add_fd_sn����fd sn���б����del_fd_snɾ��fd sn
 * ����push_packet�����Ϣ����Ϣ����,packet��fd,sn�������ó�ͨ��add_fd_sn���ӵ��б��fd sn
 */
using namespace sdk;

namespace common
{
class ClientFramesend : public FramesendBase
{
public:
	ClientFramesend(const FramesendBaseConfig& config,CommModule* server)
	:FramesendBase(config,server)
	{

	}

	virtual ~ClientFramesend(){}

	inline virtual PacketHandlerResult default_call(ServerPacket* packet)
	{
		uint64 key = sdk::MAKEINT64(packet->get_fd(),packet->get_sn());
		UnitMap::iterator iter = _units.find(key);
		if (iter != _units.end())
		{
			ClientPacket p(*packet);
			frame_send(iter->second,&p);
		}

		return DELETE_PACKET;
	}
};
}

#endif /* CLIENTFRAMESEND_H_ */

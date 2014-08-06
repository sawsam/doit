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
 * ClientFramesend 用于分帧发送给Client
 *
 * 使用方法：
 * 1 配置：通过FramesendBaseConfig设置_timeout _bufsize
 * 2 不加逻辑可以直接使用：
 * 调用add_fd_sn增加fd sn到列表或者del_fd_sn删除fd sn
 * 调用push_packet添加消息到消息队列,packet的fd,sn必须设置成通过add_fd_sn增加到列表的fd sn
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

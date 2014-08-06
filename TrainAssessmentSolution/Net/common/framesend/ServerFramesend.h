/*
 * ServerFramesend.h
 *
 *  Created on: Oct 19, 2010
 *      Author: work
 */

#ifndef SERVERFRAMESEND_H_
#define SERVERFRAMESEND_H_

#include "FramesendBase.h"
#include "tools/ServerPacket.h"

/*
 * ServerFramesend 用于分帧发送给Server
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
class ServerFramesend : public FramesendBase
{
public:
	ServerFramesend(const FramesendBaseConfig& config,CommModule* server)
	:FramesendBase(config,server)
	{}

	virtual ~ServerFramesend(){}

	//--大部分发到缓冲区中，真正发送出去是在 FramesendBase SendTimer 定时器里完成的。
	inline virtual PacketHandlerResult default_call(ServerPacket* packet)
	{
		uint64 key = sdk::MAKEINT64(packet->get_fd(),packet->get_sn());
		UnitMap::iterator iter = _units.find(key);
		if (iter != _units.end())
		{
			frame_send(iter->second,packet);
		}

		return DELETE_PACKET;
	}
};
}

#endif /* SERVERFRAMESEND_H_ */

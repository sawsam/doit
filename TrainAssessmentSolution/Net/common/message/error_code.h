#ifndef ERROR_CODE_H_
#define ERROR_CODE_H_

namespace common
{
	enum ErrorCode
	{
		E_NO_ERR = 0,

		E_PASSWORD_ERR = 1,//帐号密码错误

		E_CHECKED_EXPIRE = 2,//验证已过期

		E_SERVER_EXCEPTION = 3,//服务异常

		E_ROLE_ERROR = 4,//角色不存在

		E_ROLE_REPEAT = 5,//玩家名字重复

		E_ROOM_BUSY   = 6, //服务器繁忙，房间操作失败

		E_BIG_HORN_NOT_ENOUGH = 7,//大喇叭不足

		E_NOT_WANT_JOIN_ROOM = 8, //对方不想加入房间

		E_NOT_IN_TEAM = 9,//不在队伍，不能队伍聊天

		E_NOT_IN_ROOM = 10,//不在房间，不能房间聊天

		E_NO_RIGHT_ROOM = 11 ,//没有合适的房间

		E_ROLE_NOT_EXIST = 15,//角色不存在

		E_FRIEND_LIST_FULL = 16,//好友列表已满

		E_OPPOSITE_FRIEND_LIST_FULL = 17,//对方好友列表已满

		E_FRIEND_ALREADY = 18,//已经是好友

		E_BLACK_LIST_FULL = 19,//黑名单列表已满

		E_BLACK_ALREADY = 20,//已在黑名单中

		E_IN_BLACK = 21,//在自己的黑名单中，不能加好友

		E_IN_OPPOSITE_BLACK = 22,//在对方的黑名单中，不能加好友

		E_IN_FRIEND = 23,//在好友列表中，不能加黑名单

		E_IN_BLACK_NO_TALK = 24,//在自己的黑名单中，不能私聊

		E_IN_OPPOSITE_BLACK_NO_TALK = 25,//在对方的黑名单中，不能私聊

		E_NOT_ONLINE = 26,//对方不在线

		E_ROOM_ERROR_PASSWORD = 28, //房间密码错误

		E_ROOM_HALF_JOIN = 29, //中途不可加入

		E_ROOM_FULL = 30, //人数已经满里

		E_ROOM_NO_EXIST = 31, //房间不存在

		E_MAP_HAS_MORE_PLAYERS = 32, //线人数已达上限

		E_ROOM_ERROR_CAREER = 33, //该职业不可以进入房间

		E_ROOM_END_SPAWN = 34, //已经到里最后一个复活点，不可以中途进入

		E_ROOM_IS_CLOSE  = 35,  //房间已经在结算或者关闭状态中

		E_BAG_NOT_ENOUGH = 36, //背包空间不足

		E_ITEM_SELL_OUT = 37,//该物品已被抢购光了。

		E_ITEM_BUY_ENOUGH = 38,//您已购买完限购数量的该物品。
	};
}

#endif /* ERROR_CODE_H_ */;
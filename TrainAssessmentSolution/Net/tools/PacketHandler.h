#ifndef PACKETHANDLER_H_
#define PACKETHANDLER_H_

#include "global/base_inc.h"
#include "PacketBase.h"

namespace sdk
{
enum PacketHandlerResult
{
	/** 致命错误，使用者不返回 */
	HANDLER_ERROR = 0,
	/** 处理过程完毕后packet会被释放 */
	DELETE_PACKET = 1,
	/** 处理过程完毕后packet不会被释放，packet由使用者自由使用 */
	REUSE_PACKET = 2,
};

class PacketHandler;
/** 类成员函数 */
typedef PacketHandlerResult (PacketHandler::*PacketHandleFunction)(ServerPacket *packet); 

/** 消息包处理过程 */
class PacketHandle
{
public:
	uint32 _code;
	PacketHandleFunction _func;
	bool _enable;

	uint32 _call_count;	/**< 记录调用次数 */

public:
	PacketHandle(uint32 code, PacketHandleFunction func, bool enable) : _code(code), _func(func), _enable(enable), _call_count(0) { }

	virtual ~PacketHandle() { }
};

/** 消息包处理过程集合 */
class PacketHandleTable
{
public:
	PacketHandleTable() { }

	virtual ~PacketHandleTable() { }

public:
	/** 查找消息包对应的处理过程 */
	virtual PacketHandle * find_handle(ServerPacket *packet) = 0;
};

/**
 * 消息包处理者，用于分派消息包
 * no thread safty
 */
class PacketHandler
{
public:
	PacketHandler()
	{
	}

	virtual ~PacketHandler()
	{
	}

public:
	/**
	 * @return 0 成功, -1 失败, -101 返回的handle table为空, -102 从handle table返回的处理过程为空, > 0 是用户定义返回
	 */
	inline PacketHandlerResult dispatch_packet(ServerPacket *packet)
	{
		PacketHandleTable *table = get_handle_table();
		if (table != NULL)
		{
			PacketHandle *handle = table->find_handle(packet);
			if (handle != NULL)
			{
				handle->_call_count++;
				return (this->*(handle->_func))(packet);
			}
			else
			{
				return HANDLER_ERROR;
			}
		}
		else
		{
			return HANDLER_ERROR;
		}

		return DELETE_PACKET;
	}

	/** 返回handle table，继承者必须实现 */
	virtual PacketHandleTable * get_handle_table() = 0;
};

/** 消息包处理过程hash集合 */
class PacketHandleHashTable : public PacketHandleTable
{
private:
	typedef hash_map<uint32, PacketHandle *> HandleMap;
	HandleMap _handle_map;

	PacketHandle _default_handle;

public:
	PacketHandleHashTable()
	:_default_handle(0, NULL, true)
	{
	}

	virtual ~PacketHandleHashTable()
	{
		for (HandleMap::iterator iter = _handle_map.begin(); iter != _handle_map.end(); ++iter)
		{
			delete iter->second;
		}
		_handle_map.clear();
	}

public:
	/** 实现了查找方法，允许返回空 */
	inline virtual PacketHandle * find_handle(ServerPacket *packet)
	{
		HandleMap::iterator iter = _handle_map.find(packet->get_code());
		if (iter != _handle_map.end())
		{
			if (iter->second->_enable)
			{
				return iter->second;
			}
			else
			{
				return NULL;
			}
		}
		else if (_default_handle._func != NULL)
		{
			return &_default_handle;
		}
		else
		{
			return NULL;
		}
	}

	template<typename T>
	void insert_default_handle(T func)
	{
		insert_default_handle((PacketHandleFunction)func);
	}

	template<typename T>
	void insert_handle(uint32 code, T func)
	{
		insert_handle(code, (PacketHandleFunction)func);
	}


	/** 添加默认处理过程 */
	void insert_default_handle(PacketHandleFunction func)
	{
		_default_handle._func = func;
	}

	/** 添加消息处理过程 */
	void insert_handle(uint32 code, PacketHandleFunction func)
	{
		HandleMap::iterator iter = _handle_map.find(code);
		if (iter == _handle_map.end())
		{
			PacketHandle *handle = new PacketHandle(code, func, true);
			_handle_map.insert(pair<uint32, PacketHandle *>(code, handle));
		}
		else
		{
			cerr << __FUNCTION__ << "," << __LINE__ << ":" << "handle already exist" << endl;
			abort();
		}
	}

	/** 使消息处理过程无效 */
	void disable_handle(uint32 code)
	{
		HandleMap::iterator iter = _handle_map.find(code);
		if (iter != _handle_map.end())
		{
			iter->second->_enable = false;
		}
	}

	/** 使消息处理过程有效 */
	void enable_handle(uint32 code)
	{
		HandleMap::iterator iter = _handle_map.find(code);
		if (iter != _handle_map.end())
		{
			iter->second->_enable = true;
		}
	}
};
}

#endif /* PACKETHANDLER_H_ */

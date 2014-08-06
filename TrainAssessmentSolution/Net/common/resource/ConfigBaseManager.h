#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "ConfigBase.h"
#include "ConfigBaseDefine.h"
#include "BaseXml.h"

//--配置文件加载管理器的基类，其他各个工程继承这个基类，并重写 Initialize 接口,和各自的 LoadAllConfig 接口

namespace common
{

class ConfigBaseManager
{

public:
	ConfigBaseManager()
	{

	}

	virtual ~ConfigBaseManager()
	{

	}

public:

	//--注册接口
	inline bool register_config_instance(ConfigType ty,ConfigBase* instance,bool is_register = true)
	{
		if(!instance)
		{
			PRINT_ERROR_INFO;
			return false;
		}

		map<ConfigType,ConfigBase*>::iterator iter = _config_instance_map.find(ty);
		if(iter != _config_instance_map.end())
		{
			instance->_is_register = is_register;
			return true;
		}

		instance->_is_register   = is_register;
		_config_instance_map[ty] = instance;

		return true;
	}

	//--反注册接口1
	inline void unregister_config_instance(ConfigType ty)
	{
		_config_instance_map[ty]->_is_register = false;
	}

	//--反注册接口2
	inline void unregister_config_instance(const ConfigBase *instance)
	{
		map<ConfigType,ConfigBase*>::iterator iter;
		for(iter = _config_instance_map.begin(); iter!= _config_instance_map.end(); ++iter)
		{
			if ( iter->second == instance)
			{
				iter->second->_is_register = false;
			}
		}
	}

	//--native 接口
	inline bool load_all_config(map<int,string> const &path_list)
	{
		//if(path_list.size() != _config_instance_map.size())
		//{
			//return false;
		//}

		_load_error_info.clear();

		bool is_all_load = true;

		map<int,string> path(path_list);
		map<int,string>::iterator iter;
		for (iter = path.begin(); iter != path.end(); ++iter)
		{
			map<ConfigType,ConfigBase*>::iterator it = _config_instance_map.find(static_cast<ConfigType>(iter->first));
			if(it != _config_instance_map.end() && it->second->_is_register == true)
			{
				if(!it->second->load(iter->second.c_str()))
				{
					is_all_load = false;
					_load_error_info.push_back(iter->second.c_str());
				}
			}
		}

		return is_all_load;
	}

	inline ConfigBase* get_instance_by_type(ConfigType ty)
	{
		assert(_config_instance_map[ty]);
		assert(_config_instance_map[ty]->_is_register);
		return _config_instance_map[ty];
	}

	template<class T>
	T* get_config(ConfigType ty)
	{
		return (T*)get_instance_by_type(ty);
	}

	vector<string>* get_load_error_info()
	{
		return &_load_error_info;
	}

	//--C++接口
	virtual bool load_all_config()
	{
		return true;
	}

	virtual bool initialize() = 0;

protected:
	map<ConfigType,ConfigBase*> _config_instance_map;
	vector<string> _load_error_info;

};


}

#endif


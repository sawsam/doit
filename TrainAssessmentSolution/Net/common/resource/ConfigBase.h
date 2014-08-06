#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <string>
#include <iostream>

#include "ConfigBaseDefine.h"

namespace common
{

class ConfigBase
{
public:

	ConfigBase(ConfigType ty):_config_type(ty),_is_register(false)
	{

	}
	virtual ~ConfigBase()
	{

	}

	inline virtual ConfigType get_type()
	{
		return _config_type;
	}

public:

	inline virtual bool load(const string &path) = 0;

public:
	ConfigType  _config_type;
	bool        _is_register;
};
}


#endif
#ifndef SERVERBASECONFIG_H_
#define SERVERBASECONFIG_H_

#include "communicate/CommConfig.h"
#include "enginebase/EngineBaseConfig.h"
#include "common/framesend/FrameSendBaseConfig.h"

using namespace sdk;

namespace common
{
struct ServerBaseConfig
{
	ServerBaseConfig()
	{
	}

	ServerBaseConfig(const ServerBaseConfig& config)
	:_commconfig(config._commconfig),_engineconfig(config._engineconfig),_framesendconfig(config._framesendconfig)
	{
	}
	ServerBaseConfig& operator=(const ServerBaseConfig& config)
	{
		_commconfig = config._commconfig;
		_engineconfig = config._engineconfig;
		_framesendconfig = config._framesendconfig;
		return *this;
	}

	CommConfig _commconfig;
	EngineBaseConfig _engineconfig;
	FramesendBaseConfig _framesendconfig;
};
}

#endif /* SERVERBASECONFIG_H_ */
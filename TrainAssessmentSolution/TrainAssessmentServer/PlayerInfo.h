#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_

#include "global/base_inc.h"

struct PlayerInfo
{
	PlayerInfo(string name, string passwd)
	:_name(name),_passwd(passwd),_fd(0),_sn(0)
	{
	}

	string _name;
	string _passwd;
	int32 _fd;
	int32 _sn;
};
typedef boost::shared_ptr<PlayerInfo> PlayerInfoPtr;

#endif /* PLAYERINFO_H_ */
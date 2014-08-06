#ifndef USERINFO_H_
#define USERINFO_H_

#include "global/base_inc.h"

struct UserInfo
{
	UserInfo(string user_name, string passwd, int number, string name, string department)
	:_user_name(user_name),_passwd(passwd),_fd(0),_sn(0),_number(number),_name(name),_department(department)
	{
	}

	string _user_name;
	string _passwd;
	int32 _fd;
	int32 _sn;

	int _number;
	string _name;
	string _department;
};
typedef boost::shared_ptr<UserInfo> UserInfoPtr;

#endif /* USERINFO_H_ */
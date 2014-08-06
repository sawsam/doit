#ifndef CONFIG_DEFINE_H
#define CONFIG_DEFINE_H

//--配置系统需要用到的头文件都放在这里

#include "boost/shared_ptr.hpp"
#include <string>
#include <iostream>
#include <global/define_inc.h>

using namespace std;

#include "../rapidxml/rapidxml.hpp"       
#include "../rapidxml/rapidxml_utils.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#include "../rapidxml/rapp.hpp"

using namespace rapp;

namespace common
{  
/* 
	1、配置文件的全部结构都定义在这里
	2、配置类的类型定义按字段分开(按各个服务器逻辑前后分段)
       mapserver(即ue3工程用到的)                     0   - 100
	   gateserver                                   101 - 200
	   worldserver                                  201 - 300
	   logserver                                    301 - 400
	   managerserver                                401 - 500
	   daemon                                       501 - 600
	   common                                       601 -     (公共资源管理)

	3、每个type 字段前用各自的服务器名称首3个字母大写开头
*/

enum ConfigType
{
	CONFIG_NULL          = -1,


	MAP_CONFIG_BASE      = 0,
	MAP_CONFIG_CLIENT    = 1,
	MAP_CONFIG_ATTR      = 2,
	MAP_CONFIG_LEVEL     = 3,
	MAP_CONFIG_MAPID     = 4,

	MAP_CONFIG_AVATAR_FIRST		=	5,
	MAP_CONFIG_AVATAR_THIRD		=	6,

	MAP_CONFIG_STDITEM	 = 7,

	MAP_CONFIG_INVENTORY		=	8,
	MAP_CONFIG_ARMOR			=	9,
	MAP_CONFIG_ROOM				=	10,
	MAP_CONFIG_PVEROOM			=   11,

	GAT_CONFIG_XXXX      = 101,


	WOR_CONFIG_XXXX      = 201,

	LOG_CONFIG_XXXX      = 301,

	MAN_CONFIG_XXXX      = 401,

	DAE_CONFIG_XXXX      = 501,

	COM_CONFIG_BASE      = 601, //--base.xml

};


}


#endif
#ifndef CONFIG_DEFINE_H
#define CONFIG_DEFINE_H

//--����ϵͳ��Ҫ�õ���ͷ�ļ�����������

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
	1�������ļ���ȫ���ṹ������������
	2������������Ͷ��尴�ֶηֿ�(�������������߼�ǰ��ֶ�)
       mapserver(��ue3�����õ���)                     0   - 100
	   gateserver                                   101 - 200
	   worldserver                                  201 - 300
	   logserver                                    301 - 400
	   managerserver                                401 - 500
	   daemon                                       501 - 600
	   common                                       601 -     (������Դ����)

	3��ÿ��type �ֶ�ǰ�ø��Եķ�����������3����ĸ��д��ͷ
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
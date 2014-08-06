/********************************************************************
	created:	2013-4-22   10:54
	filename: 	datatype_define.h
	file path:	...\sdk\common\shared
	author:		hc
	
	purpose:	
*********************************************************************/

#ifndef DATATYPE_DEFINE_H_
#define DATATYPE_DEFINE_H_

namespace common
{

enum ModuleDefine
{
	GATE = 1,
	WORLD = 2,
	MAP = 3,
	MANAGER = 4,
	LOGGER = 5,
	LOGIN = 6,
	FB = 7,
	ROUTE = 8,
};

#define ONE_SECOND_MILLISECONDS 1000
#define ONE_MINUTE_SECONDS 60
#define ONE_DAY_SECONDS 86400


enum ChannelModel
{
	MODEL_NONE = 0,
	MODEL_PVE = 1,
	MODEL_PVP = 2,
};

enum PVPModel
{
	PVP_NONE         = 0,   
	PVP_TEAM         = 1,    //团队作战
	PVP_PERSON       = 2,    //个人竞技
	PVP_ANNIHILATION = 3,    //歼灭模式
	PVP_OCCUPY       = 4,    //占领模式
	PVP_BLAST        = 5,    //爆破模式
};

}


#endif // DATATYPE_DEFINE_H_
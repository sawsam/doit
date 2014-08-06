#ifndef CONFIGSERVERADDRESS_H__
#define CONFIGSERVERADDRESS_H__


#include "ConfigBase.h"
#include "json.h"

namespace common
{

struct WorldXml
{
	string _addr;
	uint16 _port;

	WorldXml():_addr(""),_port(0)
	{

	}
};

typedef boost::shared_ptr<WorldXml> WorldXmlPtr;

struct GateXml
{
	string _addr;
	string _name;
	uint16 _idx;
	uint16 _client_port;

	GateXml():_addr(""),_name(""),_idx(0),_client_port(0)
	{

	}
};

typedef boost::shared_ptr<GateXml> GateXmlPtr;

struct LoginXml
{
	string _addr;
	uint16 _s_port;
	uint16 _c_port;

	LoginXml():_addr(""),_s_port(0),_c_port(0)
	{

	}
};

struct DbXml
{
	string _ip;
	uint16 _port;
	string _db_name;
	string _db_user;
	string _db_pass;

	DbXml():_ip(""),_port(0),_db_name(""),_db_user(""),_db_pass("")
	{

	}
};

typedef boost::shared_ptr<DbXml> DbXmlPtr;

struct LogItem
{
	uint16 _index;
	string _ip;
	uint16 _port;
	string _name;
	string _user;
	string _pass;

	LogItem():_index(0),_ip(""),_port(0),_name(""),_user(""),_pass("")
	{

	}
};

typedef boost::shared_ptr<LogItem> LogItemPtr;

struct LogXml
{
	string _ip;
	uint16 _port;
	map<uint16,LogItemPtr> _item_map;
};

typedef boost::shared_ptr<LogXml> LogXmlPtr;



class BaseXml : public ConfigBase
{
private:
	BaseXml(ConfigType ty):ConfigBase(ty),_map_address("")
	{

	}

	virtual ~BaseXml()
	{

	}

public:
	static BaseXml* instance()
	{
		static BaseXml base(COM_CONFIG_BASE);
		return &base;
	}

public:

	virtual bool load(const string &path)
	{
		{
			try
			{
				rapp_document doc;
				doc.load_file(path);

				rapp_element root = doc();

				//--world
				rapp_element world = root("world");
				_world_info._addr = world["addr"].to_string();
				_world_info._port = world("port").to_int32();

				//--map
				rapp_element map = root("map");
				_map_address = map["addr"].to_string();

				//--db
				rapp_for_each_name(item,"db",root)
				{
					_db_info._ip      = item("ip").to_string();
					_db_info._port    = item("port").to_uint32();
					_db_info._db_name = item("db_name").to_string();
					_db_info._db_user = item("usr").to_string();
					_db_info._db_pass = item("pass").to_string();
				}

				//-log
				rapp_for_each_name(item,"log",root)
				{
					rapp_for_each_name(server,"server",item)
					{
						_log_info._ip   = server("ip").to_string();
						_log_info._port = server("port").to_uint32();
					}

					rapp_for_each_name(database,"database",root("log"))
					{
						rapp_for_each_name(item,"item",database)
						{
							LogItemPtr log(new LogItem());

							log->_index = item["index"].to_uint32();
							log->_ip    = item["ip"].to_string();
							log->_port  = item["port"].to_uint32();
							log->_name  = item["name"].to_string();
							log->_user  = item["user"].to_string();
							log->_pass  = item["pass"].to_string();

							_log_info._item_map[log->_index] = log;
						}
					}
				}
			}
			catch (std::exception &e)
			{
				LOG(e.what());
				return false;
			}

			return true;
		}
	}

	virtual bool load_json(const string &path)
	{
		std::ifstream is; 
		try
		{
			// 解析json用Json::Reader  
			Json::Reader reader;  
			// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...  
			Json::Value root;         
 
			is.open(path, std::ios::binary);    
			if (reader.parse(is, root) == false)  
			{  
				PRINT_ERROR_INFO;
				return false;
			}

			//--world 
			_world_info._addr = root["WorldIP"].asString();
			_world_info._port = (uint16)atoi(root["WorldPort"].asCString());

			//--map
			_map_address = root["MapIP"].asString();

			//-log
			_log_info._ip   = root["LogIP"].asString();
			_log_info._port = (uint16)atoi(root["LogPort"].asCString());

			//--db
			Json::Value db_info = root["MongoDBInfo"];  
			_db_info._ip      = db_info["IP"].asString();
			_db_info._port    = (uint16)atoi(db_info["Port"].asCString());
			_db_info._db_name = db_info["DBName"].asString();
			_db_info._db_user = db_info["DBUser"].asString();
			_db_info._db_pass = db_info["DBPass"].asString();

			////-log
			//Json::Value log_info = root["LogInfo"];
			//_log_info._ip   = log_info["LogIP"].asString();
			//_log_info._port = (uint16)atoi(log_info["LogPort"].asCString());

			//Json::Value log_db_info = root["LogDBInfo"];
			//uint16 log_db_size = log_db_info.size();

			//for(uint16 i = 0; i < log_db_size; ++i)  
			//{  
			//	LogItemPtr log(new LogItem());

			//	log->_index = log_db_info[i]["Index"].asUInt();
			//	Json::Value mysql_db_info = log_db_info[i]["MysqlDBInfo"];

			//	log->_ip    = mysql_db_info["IP"].asString();
			//	log->_port  = (uint16)atoi(mysql_db_info["Port"].asCString());
			//	log->_name  = mysql_db_info["DBName"].asString();
			//	log->_user  = mysql_db_info["DBUser"].asString();
			//	log->_pass  = mysql_db_info["DBPass"].asString();

			//	_log_info._item_map[log->_index] = log;
			//}

			is.close();  
		}
		catch (std::exception &e)
		{
			LOG(e.what());
			is.close(); 
			return false;
		}

		return true;
	}



public:

	WorldXml               _world_info;
	string   _map_address;
	DbXml    _db_info;
	LogXml   _log_info;

};

typedef boost::shared_ptr<BaseXml> BaseXmlPtr;

}


#endif // CONFIGSERVERADDRESS_H__


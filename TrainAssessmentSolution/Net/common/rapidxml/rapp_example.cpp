#include "stdlib.h"
#include <iostream>
#include <time.h>
#include "windows.h"

using namespace std;


#include "rapidxml.hpp"       
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

//--rapidxml 使用实例，具体各个工程的使用方法看 svn doc目录下 xml配置文件框架使用说明.doc 
void test_read_log_xml(const char* path,const int count, DWORD* time)

{

#if 0

	const char* path = "loger.xml";



	rapidxml::file<> fdoc(path);



	cout<<fdoc.data()<<endl;

	rapidxml::xml_document<> doc;



	doc.parse<0>(fdoc.data());

	cout<<doc.name()<<endl;



	rapidxml::xml_node<char>* root = doc.first_node();

	cout<<root->name()<<endl;



	rapidxml::xml_node<char> * priority = root->first_node("priority");



	for(rapidxml::xml_attribute<char> * attr = priority->first_attribute("path");attr != NULL; attr = attr->next_attribute())

	{

		cout<<attr->name()<<endl;

		cout<<attr->value()<<endl;

	}



	for(rapidxml::xml_node<char> * Item = priority->first_node("item"); Item != NULL; Item = Item->next_sibling())

	{

		for(rapidxml::xml_attribute<char> * attr = Item->first_attribute("source");attr != NULL; attr = attr->next_attribute())

		{

			cout<<attr->name()<<endl;

			cout<<attr->value()<<endl;

		}

	}



	rapidxml::xml_node<char> * group = root->first_node("group");



	for(rapidxml::xml_attribute<char> * attr = group->first_attribute("work_count");attr != NULL; attr = attr->next_attribute())

	{

		cout<<attr->name()<<endl;

		cout<<attr->value()<<endl;

	}



	for(rapidxml::xml_node<char> * Item = group->first_node("item"); Item != NULL; Item = Item->next_sibling())

	{

		for(rapidxml::xml_attribute<char> * attr = Item->first_attribute("id");attr != NULL; attr = attr->next_attribute())

		{

			cout<<attr->name()<<endl;

			cout<<attr->value()<<endl;

		}

	}



	rapidxml::xml_node<char> * record = root->first_node("record");

	for(rapidxml::xml_node<char> * Item = record->first_node("item"); Item != NULL; Item = Item->next_sibling())

	{

		for(rapidxml::xml_attribute<char> * attr = Item->first_attribute("id");attr != NULL; attr = attr->next_attribute())

		{

			cout<<attr->name()<<endl;

			cout<<attr->value()<<endl;

		}



		rapidxml::xml_node<char> * sql = Item->first_node("sql");

		rapidxml::xml_attribute<char> * attr = sql->first_attribute("content");

		cout<<attr->value()<<endl;

	}

#endif

#if 0

	int count = 1;

	int64 time1 = 0;

	time1 = 0;

	int64 time2 = 0;

	time2 = 0;

#endif

#if 0



	SimpleTimer s1;

	s1.start();



	try

	{

		for(int i = 0;i < count;++i)

		{

			rapp::rapp_document doc;

			const char* path = "1test.xml";

			doc.load_file(path);

			//doc.parse();



			rapp::rapp_element root = doc();



			rapp::rapp_element start_day = root("start_day");



			cout << start_day["year"].to_int32() << ", ";

			cout << start_day["month"].to_int32() << ", ";

			cout << start_day["day"].to_int32() << endl;



			cout << root("stop_day")["year"].to_int32() << ", ";

			cout << root("stop_day")["month"].to_int32() << ", ";

			cout << root("stop_day")["day"].to_int32() << endl;



			cout << root("dati")["max_num"].to_int32() << ", ";

			cout << root("dati")["text"].to_string() << endl;



			rapp_for_each_name(item, "item", root("rangeaward"))

			{

				cout << item["range"].to_uint32() << ", ";

				cout << item["item"].to_uint32() << ", ";

				cout << item["count"].to_uint32() << endl;

			}

		}

	}

	catch (rapp::exception &ex)

	{

		cout << ex.what() << endl;

	}



	s1.stop();

	cout<<endl;

	time1 = s1.interval()/count;



#endif



#if 0



	SimpleTimer s2;

	s2.start();

	try

	{

		for(int i =0 ;i < count;++i)

		{

			txpp::document doc;



			doc.load_file("test.xml");

			//doc.parse(xml);



			txpp::element root = doc();



			txpp::element start_day = root("start_day");

			cout << start_day["year"].to_int32() << ", ";

			cout << start_day["month"].to_int32() << ", ";

			cout << start_day["day"].to_int32() << endl;



			cout << root("stop_day")["year"].to_int32() << ", ";

			cout << root("stop_day")["month"].to_int32() << ", ";

			cout << root("stop_day")["day"].to_int32() << endl;



			cout << root("dati")["max_num"].to_int32() << ", ";

			cout << root("dati")["text"].to_string() << endl;



			txpp_for_each_name(item, "item", root("rangeaward"))

			{

				cout << item["range"].to_uint32() << ", ";

				cout << item["item"].to_uint32() << ", ";

				cout << item["count"].to_uint32() << endl;

			}

		}

	}

	catch (txpp::exception &ex)

	{

		cout << ex.what() << endl;

	}



	s2.stop();

	time2 = s2.interval()/count;



	cout<<endl;

	cout<<"rapp average use time :" <<time1<<"us/once"<<endl;

	cout<<"txpp average use time :" <<time2<<"us/once"<<endl;



#endif



//--loger.xml 取读，loger.xml 基本包含了常见的节点格式



#if 0

	try

	{

		for(int i = 0;i < count;++i)

		{

			rapp::rapp_document doc;

			const char* path = "loger.xml";

			doc.load_file(path);



			rapp::rapp_element root = doc();



			//--priority

			rapp::rapp_element priority = root("priority");

			cout << priority["path"].to_string() << ", ";

			cout << priority["sys_source"].to_string() <<endl;



			rapp_for_each_name(item,"item",root("priority"))

			{

				cout << item["source"].to_uint32() << ", ";

				cout << item["folder_name"].to_string() << endl;

			}



			//--record

			rapp::rapp_element record = root("record");



			rapp_for_each_name(item,"item",root("record"))

			{

				cout << item["id"].to_uint32() << ", ";

				cout << item["batch"].to_uint32() << ", ";

				cout << item["group_ide"].to_uint32(23) << endl;

				cout << item["group_id"].to_int32() << endl;

				cout << item["group_id"].to_int64() << endl;

				cout << item["group_id"].to_uint64() << endl;

				cout << item["group_eid"].to_double(12.999) << endl;



				rapp_for_each_name(sql,"sql",item)

				{

					cout << sql["content"].to_string() << endl;

				}

			}

		}

	}

	catch (rapp::exception &ex)

	{

		cout << ex.what() << endl;

	}



#endif



#if 0

	try

	{

		for(int i = 0;i < count;++i)

		{

			txpp::document doc;

			const char* path = "loger.xml";

			doc.load_file(path);



			txpp::element root = doc();



			//--priority

			txpp::element priority = root("priority");

			cout << priority["path"].to_string() << ", ";

			cout << priority["sys_source"].to_string() <<endl;



			txpp_for_each_name(item,"item",root("priority"))

			{

				cout << item["source"].to_uint32() << ", ";

				cout << item["folder_name"].to_string() << endl;

			}



			//--record

			txpp::element record = root("record");



			txpp_for_each_name(item,"item",root("record"))

			{

				cout << item["id"].to_uint32() << ", ";

				cout << item["batch"].to_uint32() << ", ";

				cout << item["group_id"].to_uint32(343) << endl;

				//cout << item["group_ide"].to_uint32() << endl;



				txpp_for_each_name(sql,"sql",item)

				{

					cout << sql["content"].to_string() << endl;

				}

			}

		}

	}

	catch (txpp::exception &ex)

	{

		cout << ex.what() << endl;

	}



#endif



#if 0

	const string xml =

	"<secret>"

	"	<start_day year=\"2010\" month=\"7\" day=\"26\" />"

	"	<stop_day year=\"2010\" month=\"9\" day=\"16\" />"

	"	<answer_time start_hour=\"12\" start_minite=\"0\" stop_hour=\"16\" stop_minite=\"0\" />"

	"	<award_time start_hour=\"16\" start_minite=\"0\" stop_hour=\"23\" stop_minite=\"50\" />"

	"	<dati max_num=\"20\" text=\"this is a message\" />"

	"	<rangeaward>"

	"		<item range=\"1\" item=\"601009500\" count=\"3\" />"

	"		<item range=\"1\" item=\"601009500\" count=\"1\" />"

	"		<item range=\"2\" item=\"601009500\" count=\"3\" />"

	"		<item range=\"3\" item=\"601009500\" count=\"3\" />"

	"		<item range=\"4\" item=\"610908610\" count=\"1\" />"

	"	</rangeaward>"



	"</secret>";



	try

	{

		for(int i = 0;i < count;++i)

		{

			rapp::rapp_document doc;

			//const char* path = "loger.xml";

			doc.parse(xml);



			rapp::rapp_element root = doc();



			rapp_element start_day = root("start_day");

			cout << start_day["yeare"].to_int32() << ", ";

			cout << start_day["month"].to_int32() << ", ";

			cout << start_day["day"].to_int32() << endl;



			cout << root("stop_day")["year"].to_int32() << ", ";

			cout << root("stop_day")["month"].to_int32() << ", ";

			cout << root("stop_day")["day"].to_int32() << endl;



			cout << root("dati")["max_num"].to_int32() << ", ";

			cout << root("dati")["text"].to_string() << endl;



			rapp_for_each_name(item, "item", root("rangeaward"))

			{

				cout << item["range"].to_uint32() << ", ";

				cout << item["item"].to_uint32() << ", ";

				cout << item["count"].to_uint32() << endl;

			}

		}

	}

	catch (rapp::exception &ex)

	{

		cout << ex.what() << endl;

	}



#endif



}


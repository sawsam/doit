
#ifndef RAPP_HPP_
#define RAPP_HPP_

#include <global/define_inc.h>
#include <boost/shared_ptr.hpp>
#include "rapidxml_utils.hpp"
#include <stdlib.h>
using namespace rapidxml;

namespace rapp
{

/*
 * 1、默认都是以char作为模板参数
 * 2、智能指针的使用
 * 3、throw() 函数(此函数不抛出异常，需要你自己捕捉)
 */

typedef boost::shared_ptr<file<char> > filePtr;

struct rapp_node
{
	xml_node<char> *_node;

	rapp_node(xml_node<char> *node):_node(node)
	{

	}

	virtual ~rapp_node()
	{

	}

	void out_path(ostream &out) const
	{
		if(_node == NULL)
		{
			cout<<"<null>"<<endl;;
			return;
		}

		if(_node->parent() != NULL)
		{
			rapp_node(_node->parent()).out_path(out);
			out << " -> ";
		}

		if(_node->name())
		{
			out << _node->name();
		}
		else
		{
			out<< "<unknow>";
		}
	}
};

inline ostream & operator <<(ostream &out,const rapp_node &node)
{
	node.out_path(out);
	return out;
};

struct exception : public std::exception
{
	string _detail;

	exception(const string &detail) throw() :_detail(detail)
	{

	}

	virtual ~exception() throw()
	{

	}

	virtual const char* what() const throw()
	{
		return _detail.c_str();
	}
};

//--xml_attribute 本身是可以取得节点的名字的,某个节点的属性值
struct rapp_attribute
{
	//--在_elem这个节点中字段为_name的属性值，这里所不能用xml_attribute，以为xml_attribute是指单个属性字段
	//--这里需要用某个点，然后按name取该字段的值

	xml_node<char> * _elem;
	string _name;
	string _file_name;

	rapp_attribute(xml_node<char> * elem,const string &name,const string &file):_elem(elem),_name(name),_file_name(file)
	{

	}

	rapp_attribute(const rapp_attribute &attr):_elem(attr._elem),_name(attr._name),_file_name(attr._file_name)
	{

	}

	virtual ~rapp_attribute()
	{

	}

	rapp_attribute & operator = (const rapp_attribute &attr)
	{
		_elem      = attr._elem;
		_name      = attr._name;
		_file_name = attr._file_name;
		return *this;
	}

	bool empty()
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		return name_attr == NULL;
	}

	void error_not_found() //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "attribute '" << _name << "' not found or empty" << endl;
		oss << "File: " << _file_name << endl;
		oss << "Path: " << rapp_node(_elem) << endl;
		throw exception(oss.str());
	}

	void error_convert_failed() //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "convert attribute '" << _name << "' failed" << endl;
		oss << "File: " << _file_name << endl;
		oss << "Value: " << "<empty>" << endl;
		oss << "Path: " << rapp_node(_elem) << endl;
		throw exception(oss.str());
	}

	void error_convert_failed(const char *value) //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "convert attribute '" << _name << "' failed" << endl;
		oss << "File: " << _file_name << endl;
		oss << "Value: " << (value != NULL ? value : "<empty>") << endl;
		oss << "Path: " << rapp_node(_elem) << endl;
		throw exception(oss.str());
	}

	int32_t to_int32()
	{
		return static_cast<int32>(to_int64());
	}

	int32_t to_int32(int32_t defval)
	{
		return static_cast<int32>(to_int64(defval));
	}

	uint32_t to_uint32()
	{
		return static_cast<uint32>(to_uint64());
	}

	uint32_t to_uint32(uint32_t defval)
	{
		return static_cast<uint32>(to_uint64(defval));
	}

	int64_t to_int64()
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			error_not_found();
		}

		string s(name_attr->value());
		const string *value(&s);

		if (value->size() == 0)
		{
			error_convert_failed();
		}
		char *end = NULL;
		int64_t res = strtol(value->c_str(), &end, 10);
		if (*end != '\0')
		{
			error_convert_failed(value->c_str());
		}
		return res;
	}

	int64_t to_int64(int64_t defval)
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			error_not_found();
		}

		string s(name_attr->value());
		const string *value(&s);

		if ( value->size() > 0)
		{
			char *end = NULL;
			int64_t res = strtol(value->c_str(), &end, 10);
			if (*end != '\0')
			{
				error_convert_failed(value->c_str());
			}
			return res;
		}
		else
		{
			return defval;
		}
	}

	uint64_t to_uint64()
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			error_not_found();
		}

		string s(name_attr->value());
		const string *value(&s);

		if (value->size() == 0)
		{
			error_convert_failed();
		}
		char *end = NULL;
		uint64_t res = strtoul(value->c_str(), &end, 10);
		if (*end != '\0')
		{
			error_convert_failed(value->c_str());
		}
		return res;
	}

	uint64_t to_uint64(uint64_t defval)
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr != NULL)
		{
			string s(name_attr->value());
			const string *value(&s);

			if (value->size() > 0)
			{
				char *end = NULL;
				uint64_t res = strtoul(value->c_str(), &end, 10);
				if (*end != '\0')
				{
					error_convert_failed(value->c_str());
				}
				return res;
			}
		}
		return defval;
	}

	double to_double()
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			error_not_found();
		}

		string s(name_attr->value());
		const string *value(&s);

		if (value->size() == 0)
		{
			error_convert_failed();
		}
		char *end = NULL;
		double res = strtod(value->c_str(), &end);
		if (*end != '\0')
		{
			error_convert_failed(value->c_str());
		}
		return res;
	}

	double to_double(double defval)
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr != NULL)
		{
			string s(name_attr->value());
			const string *value(&s);

			if (value->size() > 0)
			{
				char *end = NULL;
				double res = strtod(value->c_str(), &end);
				if (*end != '\0')
				{
					error_convert_failed(value->c_str());
				}
				return res;
			}
		}
		return defval;
	}

	string to_string()
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			error_not_found();
		}

		string s(name_attr->value());
		const string *value(&s);
		return *value;
	}

	string to_string(const string &defval)
	{
		xml_attribute<char> * name_attr = _elem->first_attribute(_name.c_str());
		if(name_attr == NULL)
		{
			string s(name_attr->value());
			const string *value(&s);

			if (value != NULL)
			{
				return *value;
			}
		}
		return defval;
	}
};

//txpp 中 TiXmlElement 已经重载来 = 运算符，xml_node 是没有重载这个运算符
struct rapp_element    //--节点数据
{
	xml_node<char> * _elem;
	string _file_name;

	rapp_element(xml_node<char> * elem):_elem(elem)
	{

	}

	rapp_element(xml_node<char> * elem,const string &name):_elem(elem),_file_name(name)
	{

	}

	rapp_element(const rapp_element &elem) :_elem(elem._elem),_file_name(elem._file_name)
	{

	}

	virtual ~rapp_element()
	{

	}

	//--这里可能有问题,xml_node 是没有重载=操作符的
	rapp_element & operator = (const rapp_element &elem)
	{
		_elem = elem._elem;
		return *this;
	}

	void error_not_found() //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "child element not found" << endl;
		oss << "File: " << _file_name << endl;
		oss << "Path: " << rapp_node(_elem) << endl;
		throw exception(oss.str());
	}

	void error_not_found(const char *name) //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "child element '" << name << "' not found" << endl;
		oss << "File: " << _file_name << endl;
		oss << "Path: " << rapp_node(_elem) << endl;
		throw exception(oss.str());
	}

	void error_convert_failed() //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << "Description: " << "convert element '" << _elem->name() << "' failed" << endl;
		oss << "Value: " << (_elem->value() != NULL ? _elem->value() : "<empty>") << endl;
		oss << "Path: " << rapp_node(_elem->parent()) << endl;
		throw exception(oss.str());
	}

	bool has_child(const char*name)
	{
		xml_node<char> *elem = _elem->first_node(name);
		if(elem != NULL)
		{
			return true;
		}
		return false;
	}

	rapp_element operator () (bool no_empty = true)
	{
		xml_node<char> *elem = _elem->first_node();
		if(elem == NULL)
		{
			if(no_empty)
			{
				error_not_found();
			}
			else
			{
				return rapp_element(NULL);
			}
		}
		return rapp_element(elem,_file_name);
	}

	rapp_element operator () (const char* name,bool no_empty = true)
	{
		xml_node<char> *elem = _elem->first_node(name);
		if(elem == NULL)
		{
			if(no_empty)
			{
				error_not_found(name);
			}
			else
			{
				return rapp_element(NULL);
			}
		}
		return rapp_element(elem,_file_name);
	}

	rapp_element operator () (const string &name,bool no_empty = true)
	{
		return (*this)(name.c_str(),no_empty);
	}

	rapp_attribute operator [] (const char* name)
	{
		return (*this)[string(name)];
	}

	rapp_attribute operator [] (const string &name)
	{
		return rapp_attribute(_elem,name,_file_name);
	}

	bool empty()
	{
		return _elem == NULL;
	}

	int32_t to_int32()
	{
		return static_cast<int32>(to_int64());
	}

	int32_t to_int32(int32_t defval)
	{
		return static_cast<int32>(to_int64(defval));
	}

	uint32_t to_uint32()
	{
		return static_cast<uint32>(to_uint64());
	}

	uint32_t to_uint32(uint32_t defval)
	{
		return static_cast<uint32>(to_uint64(defval));
	}

	int64_t to_int64()
	{
		const char *value = _elem->value();
		if (value == NULL)
		{
			error_convert_failed();
		}
		char *end = NULL;
		int64_t res = strtol(value, &end, 10);
		if (*end != '\0')
		{
			error_convert_failed();
		}
		return res;
	}

	int64_t to_int64(int64_t defval)
	{
		const char *value = _elem->value();
		if (value != NULL)
		{
			char *end = NULL;
			int64_t res = strtol(value, &end, 10);
			if (*end != '\0')
			{
				error_convert_failed();
			}
			return res;
		}
		else
		{
			return defval;
		}
	}

	uint64_t to_uint64()
	{
		const char *value = _elem->value();
		if (value == NULL)
		{
			error_convert_failed();
		}
		char *end = NULL;
		uint64_t res = strtoul(value, &end, 10);
		if (*end != '\0')
		{
			error_convert_failed();
		}
		return res;
	}

	uint64_t to_uint64(uint64_t defval)
	{
		const char *value = _elem->value();
		if (value != NULL)
		{
			char *end = NULL;
			uint64_t res = strtoul(value, &end, 10);
			if (*end != '\0')
			{
				error_convert_failed();
			}
			return res;
		}
		else
		{
			return defval;
		}
	}

	double to_double()
	{
		const char *value = _elem->value();
		if (value == NULL)
		{
			error_convert_failed();
		}
		char *end = NULL;
		double res = strtod(value, &end);
		if (*end != '\0')
		{
			error_convert_failed();
		}
		return res;
	}

	double to_double(double defval)
	{
		const char *value = _elem->value();
		if (value != NULL)
		{
			char *end = NULL;
			double res = strtod(value, &end);
			if (*end != '\0')
			{
				error_convert_failed();
			}
			return res;
		}
		else
		{
			return defval;
		}
	}

	string to_string()
	{
		const char *value = _elem->value();
		if (value == NULL)
		{
			error_convert_failed();
		}
		return string(value);
	}

	string to_string(const string &defval)
	{
		const char *value = _elem->value();
		if (value != NULL)
		{
			return string(value);
		}
		else
		{
			return defval;
		}
	}
};

//--包一个文件名传给节点打印信息
struct rapp_document
{
	filePtr _fdoc;
	rapidxml::xml_document<> _doc;
	string _file_name;

	rapp_document():_fdoc(filePtr()),_file_name("")
	{

	}

	virtual ~rapp_document()
	{

	}

	void error_load(std::exception &e) //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << e.what() << endl;
		throw exception(oss.str());
	}

	void error_not_found(std::exception &e) //throw(exception)
	{
		ostringstream oss;
		oss << endl;
		oss << e.what() << endl;
		throw exception(oss.str());
	}

	void load_file(const string &name)
	{
		try
		{
			//--相互赋值
			filePtr p(new file<char>(name.c_str()));
			_fdoc = p;
			set();
		}
		catch (std::exception &e)
		{
			error_load(e);
		}

		_file_name = name;
	}

	void load_file(const char* name)
	{
		try
		{
			filePtr p(new file<char>(name));
			_fdoc = p;
			set();
		}
		catch (std::exception &e)
		{
			error_load(e);
		}
		_file_name = name;
	}

	void set()
	{
		try
		{
			_doc.parse<0>(_fdoc-> data());
		}
		catch (std::exception &e)
		{
			error_load(e);
		}
	}

	//--解析字符串流接口
	void parse(const char *xml)
	{
		try
		{
			istringstream iss(xml);
			filePtr p(new file<char>(iss));
			_fdoc = p;
			set();
		}
		catch (std::exception &e)
		{
			error_load(e);
		}
		_file_name = "load file from db,can not get file name";
	}

	//--解析字符串流接口
	void parse(const string &xml)
	{
		try
		{
			istringstream iss(xml);
			filePtr p(new file<char>(iss));
			_fdoc = p;
			set();
		}
		catch (std::exception &e)
		{
			error_load(e);
		}
		_file_name = "load file from db,can not get file name";
	}

	//--重载() 运算符，最上层取节点接口
	rapp_element operator() ()
	{
		xml_node<> *elem = _doc.first_node();
		try
		{
			assert(elem);
		}
		catch (std::exception &e)
		{
			error_not_found(e);
		}

		return rapp_element(elem,_file_name);
	}
};

inline rapp_element iterator_begin (rapp_element parent)
{
	return rapp_element(parent._elem->first_node(),parent._file_name);
}

inline rapp_element iterator_begin (rapp_element parent,const char* name)
{
	return rapp_element(parent._elem->first_node(name),parent._file_name);
}

inline rapp_element iterator_begin (rapp_element parent,const string &name)
{
	return rapp_element(parent._elem->first_node(name.c_str()),parent._file_name);
}

inline rapp_element iterator_next (const rapp_element &elem)
{
	return rapp_element(elem._elem->next_sibling(),elem._file_name);
}

inline rapp_element iterator_next (const rapp_element &elem,const char* name)
{
	return rapp_element(elem._elem->next_sibling(name),elem._file_name);
}

inline rapp_element iterator_next (const rapp_element &elem,const string &name)
{
	return rapp_element(elem._elem->next_sibling(name.c_str()),elem._file_name);
}

#define rapp_for_each(var,node) for(rapp::rapp_element var = rapp::iterator_begin(node);var.empty() == false; var = rapp::iterator_next(var))
#define rapp_for_each_name(var, name, node) for (rapp::rapp_element var = rapp::iterator_begin(node, name); var.empty() == false; var = rapp::iterator_next(var, name))

}


#endif /* RAPP_HPP_ */

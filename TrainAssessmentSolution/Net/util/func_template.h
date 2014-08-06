#ifndef FUNC_TEMPLATE_H_
#define FUNC_TEMPLATE_H_

#include "global/base_inc.h"

namespace sdk
{
inline int64 MAKEINT64(int32 h, int32 l)
{
	return ((int64) ((uint32) l | ((uint64) h) << 32));
}

inline int32 HIINT64(uint64 s)
{
	return ((int32) (((uint64) (s) >> 32) & 0xffffffff));
}

inline int32 LOINT64(uint64 s)
{
	return ((int32) (s));
}

inline int32 MAKEINT32(uint16 h, uint16 l)
{
	return ((int32) ((uint16) l | ((uint32) h) << 16));
}

inline uint16 HIINT32(int32 s)
{
	return ((uint16) (((int32) (s) >> 16) & 0xffff));
}

inline uint16 LOINT32(int32 s)
{
	return ((uint16) (s));
}

template<class T>
inline string to_string(T value)
{
	stringstream ss;
	ss << value;

	return ss.str();
}

template<class T>
inline string to_collection_num_string(T value)
{
	stringstream ss;
	ss << "_" << value;

	return ss.str();
}

template<class T>
void queue_clean_and_free(queue<T> & q)
{
	while (!q.empty())
	{
		T t = q.front();
		q.pop();

		if (t)
		{
			delete t;
		}
	}
}


template<class T>
void deque_clean_and_free(deque<T> & q)
{
	while (!q.empty())
	{
		T t = q.front();
		q.pop_front();

		if (t)
		{
			delete t;
		}
	}
}

template<typename _Tp>
void list_clean_and_free(list<_Tp *> & container)
{
	typedef typename list<_Tp *>::iterator Iterator;

	for(Iterator it = container.begin();it!=container.end();++it)
	{
		_Tp * data = *it;

		if(data)
		{
			delete data;
		}
	}

	container.clear();
}

template<typename _Tp>
void vector_clean_and_free(vector<_Tp *> & container)
{
	typedef typename vector<_Tp *>::iterator Iterator;

	for(Iterator it = container.begin();it!=container.end();++it)
	{
		_Tp * data = *it;

		if(data)
		{
			delete data;
		}
	}

	container.clear();
}

template<typename _Key, typename _Tp>
void map_clean_and_free(map<_Key,_Tp *> & container)
{
	typedef typename map<_Key,_Tp *>::iterator Iterator;

	for(Iterator it = container.begin();it!=container.end();++it)
	{
		_Tp * data = it->second;

		if(data)
		{
			delete data;
		}
	}
	container.clear();
}

template<typename _Key, typename _Tp>
void hash_map_clean_and_free(hash_map<_Key,_Tp *> & container)
{
	typedef typename hash_map<_Key,_Tp *>::iterator Iterator;

	for(Iterator it = container.begin();it!=container.end();++it)
	{
		_Tp * data = it->second;

		if(data)
		{
			delete data;
		}

	}
	container.clear();
}

inline string create_token()
{
	static boost::uuids::random_generator gen;
	boost::uuids::uuid u = gen();
	return to_string(u);
}

inline uint32 replace(string & ori, const string & old_str, const string & new_str)
{
	uint32 replace_count = 0;

	while(true)
	{
		string::size_type pos(0);
		pos = ori.find(old_str);

		if(pos == string::npos)
		{
			break;
		}

		ori.replace(pos, old_str.length(), new_str);

		replace_count ++;
	}

	return replace_count;
}



}

#endif /* FUNC_TEMPLATE_H_ */
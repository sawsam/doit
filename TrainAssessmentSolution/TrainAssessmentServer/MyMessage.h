#ifndef MYMESSAGE_H_
#define MYMESSAGE_H_

#include "DataStruct.h"

class MyMessage
{
public:
	static MyMessage *instance()
	{
		static MyMessage message;
		return &message;
	}

	void add_notice(NoticePtr notice)
	{
		notice->_id = ++_notice_id;
		_notice.push_front(notice);
		if (notice->_complete)
		{
			_notice_count++;
		}
	}
	void del_notice(int id)
	{
		list<NoticePtr>::iterator iter = _notice.begin();
		for (; iter != _notice.end(); ++iter)
		{
			NoticePtr notice = *iter;
			if (notice->_id == id)
			{
				_notice.erase(iter);
				if (notice->_complete)
				{
					_notice_count--;
				}
				break;
			}
		}
	}
	void add_policy(NoticePtr notice)
	{
		notice->_id = ++_policy_id;
		_policy.push_front(notice);
		if (notice->_complete)
		{
			_policy_count++;
		}
	}
	void del_policy(int id)
	{
		list<NoticePtr>::iterator iter = _policy.begin();
		for (; iter != _policy.end(); ++iter)
		{
			NoticePtr notice = *iter;
			if (notice->_id == id)
			{
				_policy.erase(iter);
				if (notice->_complete)
				{
					_policy_count--;
				}
				break;
			}
		}
	}
	inline list<NoticePtr> &notice() { return _notice; }
	inline list<NoticePtr> &policy() { return _policy; }
	inline int &notice_count() { return _notice_count; }
	inline int &policy_count() { return _policy_count; }

private:
	MyMessage():_notice_id(0),_policy_id(0),_notice_count(0),_policy_count(0) {}
	~MyMessage() {}

	int _notice_id;
	int _policy_id;
	int _notice_count;
	int _policy_count;
	list<NoticePtr> _notice;
	list<NoticePtr> _policy;
};

#endif
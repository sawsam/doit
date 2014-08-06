#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_

#include "global/base_inc.h"

struct Attachment
{
	Attachment()
		:_name(""),_content(""),_complete(false)
	{
	}

	string _name;
	string _content;
	bool _complete;
};
typedef boost::shared_ptr<Attachment> AttachmentPtr;

struct Notice
{
	Notice()
		:_id(0),_title(""),_publisher(""),_tick(0),_content(""),_complete(false)
	{
	}

	void serialize(StreamWriter &w)
	{
		w.write_int32(_id);
		w.write_string(_title);
		w.write_string(_publisher);
		w.write_int32((int)_tick);
		w.write_string(_content);
		w.write_int32(_attachments.size());
		for (size_t i = 0; i < _attachments.size(); ++i)
		{
			w.write_string(_attachments[i]->_name);
		}
	}

	void unserialize(StreamReader &r)
	{
		_title = r.read_string();
		_publisher = r.read_string();
		_content = r.read_string();
		int count = r.read_int32();
		for (int i = 0; i < count; ++i)
		{
			AttachmentPtr attachment(new Attachment());
			attachment->_name = r.read_string();
			_attachments.push_back(attachment);
		}
	}

	int _id;
	string _title;
	string _publisher;
	time_t _tick;
	string _content;
	vector<AttachmentPtr> _attachments;
	bool _complete;
};
typedef boost::shared_ptr<Notice> NoticePtr;

struct TrainUnit
{
	TrainUnit()
		:_department(""),_duration(0),_start(0),_end(0)
	{

	}

	void serialize(StreamWriter &w)
	{
		w.write_string(_department);
		w.write_int32(_duration);
		w.write_int32((int)_start);
		w.write_int32((int)_end);
	}

	string _department;
	int _duration;
	time_t _start;
	time_t _end;
};
typedef boost::shared_ptr<TrainUnit> TrainUnitPtr;

struct TrainInfo
{
	TrainInfo()
		:_number(0),_discipline(""),_stage(""),_period(0)
	{

	}
	
	int _number;
	string _discipline;
	string _stage;
	int _period;
	vector<TrainUnitPtr> _trains;
};
typedef boost::shared_ptr<TrainInfo> TrainInfoPtr;

#endif
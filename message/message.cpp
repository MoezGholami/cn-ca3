#include "message.h"

message::message(void)
{
	source_ip="0";
	destination_ip="0";
	id=0;
	type=0;
	ttl=0;
	body="";
}

message::message(string sip, string dip, int mid, int mtype, int mttl, string bd)
{
	source_ip=sip;
	destination_ip=dip;
	id=mid;
	type=mtype;
	ttl=mttl;
	body=bd;
}

message message_from_string(string s)
{
	stringstream ss;
	int d;
	string temp;
	message result;

	ss<<s;

	ss>>temp;
	result.source_ip=temp;
	ss>>temp;
	result.destination_ip=temp;

	ss>>d;
	result.id=d;
	ss>>d;
	result.type=d;
	ss>>d;
	result.ttl=d;

	for(int i=0; i<5; ++i)
		deleteFirstLineOfString(s);
	result.body=s;

	return result;
}

string string_from_message(const message &m)
{
	string result="";

	result+=m.source_ip;
	result+="\n";
	result+=m.destination_ip;
	result+="\n";
	result+=toString(m.id);
	result+="\n";
	result+=toString(m.type);
	result+="\n";
	result+=toString(m.ttl);
	result+="\n";
	result+=m.body;

	return result;
}

bool operator==(const message& m, const message &n)
{
	return 
		(m.source_ip==n.source_ip) &&
		(m.destination_ip==n.destination_ip) &&
		(m.id==n.id) &&
		(m.type==n.type) &&
		//(m.ttl==n.ttl) &&
		(m.body==n.body)
		;
}

bool operator!=(const message& m, const message &n)
{
	return !(m==n);
}

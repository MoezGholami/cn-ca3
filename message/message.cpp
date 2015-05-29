#include "message.h"

message message_from_string(string s)
{
	stringstream ss;
	int d;
	string temp;
	message result;

	ss<<s;

	s>>temp;
	result.source_ip=temp;
	s>>temp;
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

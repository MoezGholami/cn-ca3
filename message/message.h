#ifndef __siamoz_cnca3_message
#define __siamoz_cnca3_message

#include <string>
#include <sstream>
#include "../util/util.h"

using namespace std;

const int null_message_type			=	0;
const int default_message_type			=	1;
const int my_ip_intro_type			=	2; //introduce my ip is..., body is empty
const int membership_query_type			=	3;
const int membership_report_type		=	4;
const int membership_leave_type			=	5;
const int router_interconnection_establish_type	=	6;
const int router_interconnection_dc_type	=	7;
const int router_interconnection_message_type 	=	8;
const int router_dvmrp_prune_type		=	9;
const int multicast_ip_intro_type		=	10;


typedef struct message
{
	public:
		message(void);
		message(string sip, string dip, int mid, int mtype, int mttl, string bd);
		string source_ip;
		string destination_ip; //multicast or unicast
		int id;	//id is not enough for identifying, should be + sender_ip,destination_ip
		int type;
		int ttl;
		string body;
} message;

message message_from_string(string s);
string string_from_message(const message &m);

const message null_message;

#endif

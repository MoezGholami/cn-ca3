#ifndef __siamoz_cnca3_message
#define __siamoz_cnca3_message

#include <string>
#include <sstream>
#include "../util/util.h"

using namespace std;

const int max_ttl				=	10;

const int null_message_type			=	0;

const int router_interconnection_establish_type	=	1;
const int router_interconnection_debug_type	=	2;
const int router_interconnection_message_type 	=	3;

const int unicast_message_type			=	10;
const int multicast_message_type		= 	11;

const int my_ip_intro_type			=	20; //introduce my ip is..., body is empty
const int multicast_ip_intro_type		=	21;

const int membership_report_type		=	30;
const int membership_is_member_type		=	31;
const int membership_i_join_u_type		=	32;
const int membership_leave_type			=	33;




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

bool operator==(const message& m, const message &n);
bool operator!=(const message& m, const message &n);

const message null_message;

#endif

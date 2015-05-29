#ifndef __siamoz_cnca3_message
#define __siamoz_cnca3_message

const int default_message_type			=	0;
const int my_ip_intro_type			=	1; //introduce my ip is..., body is empty
const int membership_query_type			=	2;
const int membership_report_type		=	3;
const int membership_leave_type			=	4;
const int router_interconnection_establish_type	=	5;
const int router_interconnection_dc_type	=	6;
const int router_interconnection_message_type 	=	7;
const int router_dvmrp_prune_type		=	8;


class message
{
	public:
		string sender_ip;
		string destination_ip; //multicast or unicast
		int id;	//id is not enough for identifying, should be + sender_ip,destination_ip
		int type;
		int ttl;
		string body;
};

#endif

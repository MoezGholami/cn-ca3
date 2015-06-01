#include "group_server_core.h"

group_server_core::group_server_core(string n, string uip, string mip, string sip, int rp)
{
	cn=new connection(rp,local_host_ip_address);
	if(!cn->is_ok_to_communicate())
	{
		delete cn;
		cn=0;
	}
	name=n;
	unicast_ip=uip;
	multicast_ip=mip;
	server_ip=sip;
	router_port=rp;
	if(cn)
	{
		cn->send_message(message(uip,"0",1,my_ip_intro_type,2,"0"));
		if(!cn->is_ok_to_communicate())
		{
			delete cn;
			cn=0;
		}
	}
}

int group_server_core::get_connection_fd(void)
{
	if(!cn || !cn->is_ok_to_communicate())
		return virgin_fd;
	return cn->get_fd();
}

group_server_core::~group_server_core(void)
{
	if(cn)
		delete cn;
	cn=0;
}

void group_server_core::handle_disconnection(void)
{
	if(cn && cn->is_ok_to_communicate())
		return ;
	if(cn)
		delete cn;
	cn=0;
	cout<<"connection corrupted. exiting...\n";
	exit(2);
}

void group_server_core::do_stdin_command(string line)
{
	stringstream ss;
	string parse;

	ss<<line;
	ss>>parse;
	if(parse=="add" || parse=="Add")
	{
		cn->send_message(message(unicast_ip, server_ip, 1, multicast_ip_intro_type, 10, name+" "+unicast_ip+" "+multicast_ip));
		handle_disconnection();
	}
	else
	{
		cout<<"command not recognized\n";
	}
}

void group_server_core::handle_connction_message(void)
{
	message receiving;
	stringstream ss;
	string parse;

	cn->get_message(receiving);
	handle_disconnection();

	ss<<receiving.body;
	ss>>parse;

	if(receiving.type!=unicast_message_type)
		return ;
	cout<<"log: unicast message got\n";
	if(parse=="synch")
	{
		cout<<"log: type=synch\n";
		cn->send_message(message(unicast_ip, receiving.source_ip, receiving.id, unicast_message_type, 10, "synch dadi!"));
	}
	else
	{
		cout<<"log: type=multicasting\n";
		cn->send_message(message(unicast_ip, multicast_ip, 1, multicast_message_type, 10, receiving.body));
	}
	handle_disconnection();
}

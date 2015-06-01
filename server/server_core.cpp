#include "server_core.h"

server_core::server_core(string uip, int rp)
{
	cn=new connection(rp,local_host_ip_address);
	if(!cn->is_ok_to_communicate())
	{
		delete cn;
		cn=0;
	}
	unicast_ip=uip;
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

int server_core::get_connection_fd(void)
{
	if(!cn || !cn->is_ok_to_communicate())
		return virgin_fd;
	return cn->get_fd();
}

server_core::~server_core(void)
{
	if(cn)
		delete cn;
	cn=0;
}

void server_core::handle_disconnection(void)
{
	if(cn && cn->is_ok_to_communicate())
		return ;
	if(cn)
		delete cn;
	cn=0;
	cout<<"log: connection corrupted. exiting...\n";
	exit(2);
}

void server_core::do_stdin_command(string line)
{
	cout<<"man dastur az to nemigiram\n";
}

void server_core::handle_connction_message(void)
{
	message receiving;
	stringstream ss;
	string parse, gname, guip, gmip;

	cn->get_message(receiving);
	handle_disconnection();

	if(receiving.type!=unicast_message_type && receiving.type!=multicast_ip_intro_type)
		return ;
	if(receiving.destination_ip!=unicast_ip)
	{
		cout<<"log: a new message got but not for me.\n";
		return ;
	}

	ss<<receiving.body;

	if(receiving.type==multicast_ip_intro_type)
	{
		ss>>gname>>guip>>gmip;
		if(name2unicast.find(gname)!=name2unicast.end())
		{
			cout<<"log: duplicate group registeration, discarded.\n";
			return ;
		}
		name2unicast[gname]=guip;
		name2multicast[gname]=gmip;
		cout<<"log: successfully registered group. name: "<<gname<<" uip: "<<guip<<" mip: "<<gmip<<endl;
	}
	else if(receiving.type==unicast_message_type)
	{
		cout<<"log: a new unicast message for me received\n";
		ss>>parse;
		if(parse=="join")
		{
			ss>>gname;
			if(name2unicast.find(gname)==name2unicast.end())
			{
				cout<<"log: i did not find any group with name: "<<gname<<" in my memory.\n";
				cn->send_message(message(unicast_ip, receiving.source_ip, receiving.id,
							unicast_message_type, max_ttl, "404"));
			}
			else
			{
				cout<<"log: i found the ip s of group "<<gname<<endl;
				cn->send_message(message(unicast_ip, receiving.source_ip, receiving.id, unicast_message_type, max_ttl,
							"found "+gname+" "+name2unicast[gname]+" "+name2multicast[gname]));
			}
		}
		else
		{
			cout<<"log: assuming we have got a show list\n";
			cn->send_message(message(unicast_ip, receiving.source_ip, receiving.id, unicast_message_type, max_ttl,
						"list "+list_of_groups()));
		}
	}
	handle_disconnection();
}

string server_core::list_of_groups(void)
{
	string result;
	result="list of groups: <name, unicast ip of group server, multicast ip of group>: \n";

	for(map<string, string>::iterator it=name2unicast.begin(); it!=name2unicast.end(); ++it)
		result+=( (it->first)+"\t"+it->second+"\t"+name2multicast[it->first]+"\n" );

	cout<<"log: list of groups where calculated, printing for you:\n"<<result<<endl<<endl;
	return result;
}

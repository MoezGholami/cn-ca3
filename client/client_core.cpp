#include "client_core.h"

client_core::client_core(string n, string uip, string sip, int rp)
{
	cn=new connection(rp,local_host_ip_address);
	if(!cn->is_ok_to_communicate())
	{
		delete cn;
		cn=0;
	}
	name=n;
	unicast_ip=uip;
	server_ip=sip;
	router_port=rp;
	last_uni_id_sent=1;
	rejoin_counter=0;
	last_rejoined=0;
	selected_gname="";
	if(cn)
	{
		cn->send_message(message(unicast_ip,"0",1,my_ip_intro_type,2,"0"));
		if(!cn->is_ok_to_communicate())
		{
			delete cn;
			cn=0;
		}
	}
}

int client_core::get_connection_fd(void)
{
	if(!cn || !cn->is_ok_to_communicate())
		return virgin_fd;
	return cn->get_fd();
}

client_core::~client_core(void)
{
	if(cn)
		delete cn;
	cn=0;
}

void client_core::handle_disconnection(void)
{
	if(cn && cn->is_ok_to_communicate())
		return ;
	if(cn)
		delete cn;
	cn=0;
	cout<<"connection corrupted. exiting...\n";
	exit(2);
}

void client_core::do_stdin_command(string line)
{
	stringstream ss;
	string parse;

	ss<<line;
	ss>>parse;

	rejoin_counter=(rejoin_counter+1)%rejoin_turnaround;
	if(rejoin_counter==0)
		try2rejoin_someone();

	if(parse=="Get" || parse=="get")
		cn->send_message(message(unicast_ip, server_ip, last_uni_id_sent, unicast_message_type, max_ttl, "show"));
	else if(parse=="sing" || parse=="Sign")
	 	exit(0);
	else if(parse=="show" || parse=="Show")
	 	show_groups();
	else if(parse=="Sync" || parse=="sync" || parse=="synch" || parse=="Synch")
	 	send_sync();
	else if(parse=="send" || parse=="Send")
		send2group(line);
	else if(parse=="select" || parse=="Select")
		select(line);
	else if(parse=="leave" || parse=="Leave")
		leave(line);
	else if(parse=="Join" || parse=="join")
		start_joining(line);
	else
		cout<<"command not recognized.\n";

	handle_disconnection();
}

void client_core::handle_connction_message(void)
{
	message receiving;
	stringstream ss;
	string parse;

	cn->get_message(receiving);
	handle_disconnection();

	if(receiving.type==multicast_message_type)
	{
		cout<<"log: a new multicast message received.\n"<<receiving.body<<endl;
		return ;
	}
	if(receiving.type==unicast_message_type && receiving.destination_ip!=unicast_ip)
	{
		cout<<"log: a new unicast message got but not for me\n";
		return ;
	}
	if(receiving.type==unicast_message_type && receiving.destination_ip==unicast_ip)
		handle_own_unicast_message(receiving);
	if(receiving.type==membership_is_member_type)
	{
		cn->send_message(message("0","0",0,membership_i_join_u_type,max_ttl,receiving.body));
		cout<<"log: membership labbayk was sent.\n";
	}
	handle_disconnection();

}

void client_core::handle_own_unicast_message(const message &receiving)
{
	if(receiving.id<last_uni_id_sent)
	{
		cout<<"log: a duplicate unicast message for me got.\n";
		return ;
	}
	last_uni_id_sent++;
	if(receiving.source_ip==server_ip)
		handle_server_message(receiving);
	else
		cout<<"log: a new unicast message got, it's mine: body: \n"<<receiving.body<<endl<<endl;
}

void client_core::show_groups(void)
{
	cout<<"log: selected group: "<<selected_gname<<endl;
	cout<<"the joint group list <name, group server unicast ip, group multicast ip>:\n";
	for(map<string, string>::iterator it=gname2unicast.begin(); it!=gname2unicast.end(); ++it)
		cout<<(it->first)<<"\t"<<(it->second)<<"\t"<<gname2multicast[it->first]<<endl;
	cout<<endl;
}

void client_core::send_sync(void)
{
	if(gname2unicast.find(selected_gname)==gname2unicast.end())
	{
		cout<<"you did not have select any group.\n";
		return ;
	}
	cn->send_message(message(unicast_ip, gname2unicast[selected_gname], last_uni_id_sent, unicast_message_type, max_ttl, "sync"));
	handle_disconnection();
	cout<<"log: message sent\n";
}

void client_core::send2group(const string &line)
{
	stringstream ss;
	string parse, sending_str;

	ss<<line;
	ss>>parse>>parse;

	if(gname2unicast.find(selected_gname)==gname2unicast.end())
	{
		cout<<"you should select a group.\n";
	}

	if(parse=="file" || parse=="File")
	{
		ss>>parse;
		if(!FileExist(parse))
		{
			cout<<"log: the file does not exist.\n";
			return ;
		}
		sending_str=wholeAsciiFile(parse);
	}
	else
		getline(ss, sending_str);
	cn->send_message(message(unicast_ip, gname2unicast[selected_gname],
				last_uni_id_sent, unicast_message_type, max_ttl, sending_str));
	handle_disconnection();
	cout<<"log: message sent\n";
}

void client_core::select(const string &line)
{
	stringstream ss;
	string parse;

	ss<<line;
	ss>>parse>>parse;

	if(gname2unicast.find(parse)==gname2unicast.end())
		cout<<"log: you have not joint in this group.\n";
	else
	{
		selected_gname=parse;
		cout<<"selection done.\n";
	}
}

void client_core::leave(const string &line)
{
	stringstream ss;
	string parse;

	ss<<line;
	ss>>parse>>parse;

	if(gname2unicast.find(parse)==gname2unicast.end())
	{
		cout<<"you have not joint in this group.\n";
		return ;
	}
	cn->send_message(message(unicast_ip, "0", 0, membership_leave_type, max_ttl, gname2multicast[parse]));
	handle_disconnection();
	gname2unicast.erase(parse);
	gname2multicast.erase(parse);
	cout<<"left the group.\n";
}

void client_core::start_joining(const string &line)
{
	stringstream ss;
	string parse;

	ss<<line;
	ss>>parse>>parse;

	if(gname2unicast.find(parse)!=gname2unicast.end())
	{
		cout<<"you have already joint in this group.\n";
		return ;
	}
	cn->send_message(message(unicast_ip, server_ip, last_uni_id_sent, unicast_message_type, max_ttl,
				"join "+parse));
	cout<<"join question sent to server\n";
}

void client_core::handle_server_message(const message &receiving)
{
	stringstream ss;
	string parse;

	ss<<receiving.body;
	ss>>parse;

	if(parse=="404")
	{
		cout<<"the group you have requested not found on server.\n";
	}
	else if(parse=="list")
	{
		//got list from server
		cout<<receiving.body<<endl;
	}
	else if(parse=="found")
	{
		string gname, guip, gmip;
		ss>>gname>>guip>>gmip;
		gname2unicast[gname]=guip;
		gname2multicast[gname]=gmip;
		cout<<"log: group properties got from group, trying to join.\n";
		try2join(gname);
	}
}

void  client_core::try2join(const string &gname)
{
	cn->send_message(message("0","0",0,membership_report_type, max_ttl, gname2multicast[gname]));
	cout<<"membership report sent for group: "<<gname<<endl;
	handle_disconnection();
}

void client_core::try2rejoin_someone(void)
{
	map<string, string>::iterator it;
	int i=0;
	for(it=gname2unicast.begin(); it!=gname2unicast.end() && i!=last_rejoined%gname2unicast.size();
			++it)
	{
		++i;
	}
	last_rejoined++;
	if(it!=gname2unicast.end())
		try2join(it->first);
}

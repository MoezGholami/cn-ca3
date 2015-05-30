#include "router_kernel.h"

router_kernel::router_kernel(void)
{
	add_pin(base_pin);
	cout<<"the base pin which clients are connected is: "<<base_pin<<endl;
}

router_kernel::~router_kernel(void)
{
	vector<pin> pin_copy=pins;
	for(unsigned i=0; i<pin_copy.size(); ++i)
		delete_pin(pin_copy[i]);
}

void router_kernel::add_pin(pin p)
{
	if(find(pins.begin(), pins.end(), p)!=pins.end())
	{
		cout<<"error: already added.\n";
		return ;
	}
	pins.push_back(p);
	pin_local_cost[p]=default_pin_cost;
	cout<<"in kernel: successfully added pin\n";
}

void router_kernel::delete_pin(pin p)
{
	if(p==base_pin)
	{
		cout<<"cannot delete base pin\n";
		return ;
	}
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"error: no such card found.\n";
		return ;
	}
	pins.erase(find(pins.begin(), pins.end(), p));
	pin_local_cost.erase(p);
	for(int i=0; i<(int)pin_connections[p].size(); ++i)
		delete (pin_connections[p][i]);
	pin_connections.erase(p);
	cout<<"in kernel: successfully deleted pin\n";
}

void router_kernel::disconnect_pin(pin p)
{
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"error: no such card found.\n";
		return ;
	}
	for(int i=0; i<(int)pin_connections[p].size(); ++i)
		delete (pin_connections[p][i]);
	pin_connections[p].clear();
	cout<<"successfully disconnected.\n";
}

void router_kernel::change_local_cost(pin p, int new_cost)
{
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"error: no such card found.\n";
		return ;
	}
	pin_local_cost[p]=new_cost;
	cout<<"successfully changed cost of pin to: "<<new_cost<<endl;
	update_pin_cost(p);	//talk to all peers to get the new cost of the pin
}

void router_kernel::update_pin_cost(pin p)
{
	for(unsigned i=0; i<pin_connections[p].size(); ++i)
		synch_connection_new_local_cost(pin_connections[p][i], pin_local_cost[p]);
	delete_corrupted_connections_of_pin(p);
}

void router_kernel::synch_connection_new_local_cost(connection *con_ptr, int cost)
{
	stringstream ss;
	string s;

	message sending("0","0",0,router_interconnection_message_type,1,"mylocalcost "+toString(cost));

	con_ptr->send_message(sending);

	if(!con_ptr->is_ok_to_communicate())
		handle_closing_connection(con_ptr);
}

void router_kernel::connect_to_router(pin mine, pin others, int other_port)
{
	message receiving;
	connection *con_ptr;

	if(find(pins.begin(), pins.end(), mine)==pins.end())
	{
		cout<<"do not have such pin: "<<mine<<endl;
		return ;
	}

	con_ptr = new connection(other_port, local_host_ip_address);

	if(!con_ptr->is_ok_to_communicate())
		return ;

	con_ptr->send_message(message("0","0",0,router_interconnection_establish_type,1,others));
	con_ptr->get_message(receiving);

	if(!con_ptr->is_ok_to_communicate())
	{
		cout<<"connection failed.\n";
		return ;
	}

	pin_connections[mine].push_back(con_ptr);
	synch_connection_new_local_cost(con_ptr, pin_local_cost[mine]);
	if(find(pin_connections[mine].begin(), pin_connections[mine].end(), con_ptr)!=pin_connections[mine].end()
			&& con_ptr->is_ok_to_communicate())
		new_client_like_connection_fd=con_ptr->get_fd();

}

int router_kernel::handle_message_of_fd(int fd)
{
	message receiving;
	connection *con_ptr=connection_ptr_of_fd(fd);

	if(con_ptr==0)
	{
		return handle_new_connection(fd);
	}
	con_ptr->get_message(receiving);
	if(con_ptr->is_closed())
	{
		handle_closing_connection(con_ptr);
		return 0;
	}

	handle_message(con_ptr, receiving);
	return 2;
}

void router_kernel::send_debug_message(pin p)
{
	message debug_message("0","0",++debug_message_count,router_interconnection_debug_type,1,"some");
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"no such pin\n";
		return ;
	}
	for(unsigned i=0; i<pin_connections[p].size(); ++i)
	{
		pin_connections[p][i]->send_message(debug_message);
	}
	delete_corrupted_connections_of_pin(p);
}

void router_kernel::show_tables(void)
{
}

void router_kernel::inform_running_port(int p)
{
	my_running_port=p;
}

int router_kernel::get_new_client_like_connection_fd()
{
	int result=new_client_like_connection_fd;
	new_client_like_connection_fd=virgin_fd;
	return result;
}

pin router_kernel::pin_of_fd(int fd)
{
	for(map<pin, vector<connection *> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i]->get_fd()==fd)
				return it->first;
	return null_pin;
}

pin router_kernel::pin_of_connection_ptr(connection *con_ptr)
{
	for(map<pin, vector<connection *> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i]==con_ptr)
				return it->first;
	return null_pin;
}

connection* router_kernel::connection_ptr_of_fd(int fd)
{
	for(map<pin, vector<connection *> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i]->get_fd()==fd)
				return ((it->second)[i]);
	return (connection *)0;
}

int router_kernel::handle_new_connection(int new_fd)
{
	message receiving;
	pin p;
	connection *con_ptr=new connection(new_fd);

	con_ptr->get_message(receiving);
	if(receiving.type==router_interconnection_establish_type)
	{
		p=receiving.body;
		if(find(pins.begin(), pins.end(), p)==pins.end())
		{
			cout<<"error on connecting, no such pin: "<<p<<endl;
			con_ptr->close();
			delete con_ptr;
			return 0;
		}
		pin_connections[p].push_back(con_ptr);
		cout<<"successfully got a new connection from router (answer)\n";
		return 1;
	}
	else if(receiving.type==my_ip_intro_type)
	{
		update_tables(con_ptr, receiving);
		pin_connections[base_pin].push_back(con_ptr);
	}
	else
	{
	}
	return 1;
}

void router_kernel::handle_closing_connection(connection *con_ptr)
{
	pin parent;
	map<string, connection *>::iterator it1;
	map<string, vector<connection *> >::iterator it2;
	unsigned it3;
	int deleting_idx=-1;

	parent=pin_of_connection_ptr(con_ptr);

	if(con_ptr->is_ok_to_communicate())
		return ;

	assert(parent!=null_pin);

	pin_connections[parent].erase(find(pin_connections[parent].begin(), pin_connections[parent].end(), con_ptr));

	for(it1=unicast_routing_table.begin();it1!=unicast_routing_table.end(); ++it1)
		if(it1->second==con_ptr)
			break;
	unicast_routing_table.erase(it1);

	for(it2=multicast_routing_table.begin(); it2!=multicast_routing_table.end() && deleting_idx==-1; ++it2)
		for(it3=0; it3<(it2->second).size() && deleting_idx==-1; ++it3)
			if((it2->second)[it3]==con_ptr)
				deleting_idx=(int)it3;

	if(deleting_idx!=-1)
		(it2->second).erase((it2->second).begin()+deleting_idx);

	delete con_ptr;
}

void router_kernel::delete_corrupted_connections_of_pin(pin p)
{
	vector<connection *>temp;
	for(unsigned i=0; i<pin_connections[p].size(); ++i)
		if(!pin_connections[p][i]->is_ok_to_communicate())
			temp.push_back(pin_connections[p][i]);
	for(unsigned i=0; i<temp.size(); ++i)
		handle_closing_connection(temp[i]);
}

void router_kernel::delete_corrupted_connections_of_all_pins(void)
{
	for(unsigned i=0; i<pins.size(); ++i)
		delete_corrupted_connections_of_pin(pins[i]);
}

void router_kernel::handle_message(connection *con_ptr, message &m)
{
	if(m.type==router_interconnection_debug_type)
	{
		cout<<"got a debug message.\n";
	}
	if(m.type==router_interconnection_message_type)
	{
		handle_router_message(con_ptr, m);
		return ;
	}
	if(m.type==unicast_message_type)
	{
		update_tables(con_ptr, m);
		handle_unicast_message(con_ptr,m);
	}
}

void router_kernel::handle_router_message(connection *con_ptr, const message &m)
{
	//int parse_int;
	string parse;
	stringstream ss;

	ss<<m.body;

	ss>>parse;

	if(parse=="mylocalcost" || parse=="finalcost")
		handle_router_cost_message(con_ptr, m);
}

void router_kernel::handle_router_cost_message(connection *con_ptr, const message &m)
{
	int parse_int;
	string parse;
	stringstream ss;

	ss<<m.body;
	ss>>parse>>parse_int;

	if(parse=="mylocalcost")
	{

		if(pin_local_cost[pin_of_connection_ptr(con_ptr)]>parse_int)
			con_ptr->set_cost(pin_local_cost[pin_of_connection_ptr(con_ptr)]);
		else
		{
			cout<<"successsfully changed cost to "<<parse_int<<endl;
			con_ptr->set_cost(parse_int);
		}

		con_ptr->send_message(message("0","0",0,router_interconnection_message_type,1,
					"finalcost "+toString(con_ptr->get_cost())));
	}
	else if(parse=="finalcost")
	{
			cout<<"successsfully changed cost to "<<parse_int<<endl;
			con_ptr->set_cost(parse_int);
	}
}

void router_kernel::update_tables(connection *con_ptr, const message &m)
{
	unicast_routing_table[m.source_ip]=con_ptr;
}

void router_kernel::handle_unicast_message(connection *con_ptr,message &m)
{
	if(--m.ttl==0)
		return ;
	if(unicast_routing_table.find(m.destination_ip)!=unicast_routing_table.end())
		unicast_routing_table[m.destination_ip]->send_message(m);
	else
		broadcast(m, con_ptr);
}

void router_kernel::broadcast(const message &m, connection *exception)
{
	for(map<pin, vector<connection *> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if(((it->second)[i])!=exception)
				((it->second)[i])->send_message(m);
	delete_corrupted_connections_of_all_pins();
}

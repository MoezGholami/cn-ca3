#include "router_kernel.h"

router_kernel(void)
{
	add_pin(base_pin);
	cout<<"the base pin which clients are connected is: "<<base_pin<<endl;
}

~router_kernel(void)
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

void router_kernel::change_local_cost(pin p, int new_cost)
{
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"error: no such card found.\n";
		return ;
	}
	pin_local_cost[p]=new_cost;
	update_pin_cost(p);	//talk to peer to get the new cost of the pin
}

void router_kernel::update_pin_cost(pin p)
{
	//TODO: complete this
	//talk to all peers to get the new cost of the pin
	//need connection of the pin
}

void connect_to_router(pin mine, pine others, int other_port)
{
	message receiving;
	connection *con_ptr;

	if(find(pins.begin(), pins.end(), mine)==pins.end())
	{
		cout<<"do not have such pin: "<<mine<<endl;
		return 0;
	}

	con_ptr = new connection(other_port);

	if(!con_ptr->is_ok_to_communicate())
		return ;

	con_ptr->get_message(receiving);

	if(!con_ptr->is_ok_to_communicate())
	{
		cout<<"connection failed.\n";
		return ;
	}

	pin_connections.push_back(con_ptr);
	update_pin_cost();
}

int router_kernel::handle_message_of_fd(int fd)
{
	message receiving;
	connection *con_ptr=connection_ptr_of_fd(fd);

	if(con_ptr==0)
	{
		return handle_new_connection(int fd);
	}
	con_ptr->get_message(receiving);
	if(con_ptr->is_closed())
	{
		handle_closing_connection(con_ptr);
		return 0;
	}

	//TODO: complete this
	return 2;
}

pin router_kernel::pin_of_fd(int fd)
{
	for(map<pin, vector<connection *> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i].get_fd()==fd)
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
			if((it->second)[i].get_fd()==fd)
				return ((it->second)[i]);
	return (connection *)0;
}

int connection::handle_new_connection(int new_fd)
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
		con_ptr->send_message(null_message);
		pin_connections[p].push_back(con_ptr);
		cout<<"successfully got a new connection from router (answer)\n";
		return 1;
	}
	else
	{
		//TODO: handle incommin connection from other types
	}
	return 1;
}

void handle_closing_connection(connection *con_ptr)
{
	pin parent;

	parent=pin_of_connection_ptr(con_ptr);

	assert(parent!=null_pin);

	pin_connections[p].erase(find(pin_connections[p].begin(), pin_connections[p].end(), con_ptr));

	delete con_ptr;
}

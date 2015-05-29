#include "router_kernel.h"

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
	if(find(pins.begin(), pins.end(), p)==pins.end())
	{
		cout<<"error: no such card found.\n";
		return ;
	}
	pins.erase(find(pins.begin(), pins.end(), p));
	pin_local_cost.erase(p);
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

int router_kernel::handle_message_of_fd(int fd)
{
	//TODO: complete this
	return 0;
					 /*
					int n, m;
					char buff_read [buffer_size], response_buff[buffer_size];
					clear_buff(buff_read, buffer_size);
					clear_buff(response_buff, buffer_size);

					n = read(it_fd, buff_read, buffer_size-1);
					if(n == 0)
					{
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						cout<<"client out"<<endl;
					}
					else if(n < 0)
					{
						cout<<"Error reading"<<endl;
					}

					//after reading successfully
					else
					{
						//after reading client message
						int s;
						string client_input = buff_read, server_reply;

						cout<<"new query got.\n";
						server_reply=shell.get_response_of_gotten_message(client_input, it_fd);
						if(server_reply.size()!=0)
						{
							s = write(it_fd, server_reply.c_str(), server_reply.size());
							if(s < 0)
								cout<<"send reply error\n";
						}
					}
					// */
}

pin router_kernel::pin_ptr_of_fd(int fd)
{
	for(map<pin, vector<connection> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i].get_fd()==fd)
				return it->first;
	return null_pin;
}

connection* router_kernel::connection_ptr_of_fd(int fd)
{
	for(map<pin, vector<connection> >::iterator it=pin_connections.begin();
			it!=pin_connections.end(); ++it)
		for(unsigned i=0; i<(it->second).size(); ++i)
			if((it->second)[i].get_fd()==fd)
				return &((it->second)[i]);
	return (connection *)0;
}

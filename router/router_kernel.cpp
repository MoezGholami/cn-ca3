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
	//TODO: what about pin connection???
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
	pin_connections.erase(p);//TODO: CAUTION: you should have a proper destructor
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

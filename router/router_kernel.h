#ifndef __siamoz_cnca3_router_kernel
#define __siamoz_cnca3_router_kernel

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "../connection/connection.h"

using namespace std;

const int default_pin_cost	=	1;

typedef int pin;


class router_kernel
{
	public:
		void add_pin(pin p);
		void delete_pin(pin p);
		void change_local_cost(pin p, int new_cost);

	private:
		vector<pin> pins; //caution: readundent but for easy checking.
		map<pin, int> pin_local_cost;
		map<pin, vector<connection> > pin_connections;

		void update_pin_cost(pin p);	//talk to peer to get the new cost of the pin
};

#endif

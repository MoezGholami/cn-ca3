#ifndef __siamoz_cnca3_router_kernel
#define __siamoz_cnca3_router_kernel

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "../connection/connection.h"

using namespace std;

const int default_pin_cost	=	1;

typedef string pin;
const pin null_pin="";


class router_kernel
{
	public:
		void add_pin(pin p);
		void delete_pin(pin p);
		void change_local_cost(pin p, int new_cost);

		//if returns 0, it means the connection is closed and
		//the fd should be removed from fd_set
		//normally returns something not equal to zero
		int handle_message_of_fd(int fd);

	private:
		vector<pin> pins; //caution: readundent but for easy checking.
		map<pin, int> pin_local_cost;
		map<pin, vector<connection> > pin_connections;

		void update_pin_cost(pin p);	//talk to peer to get the new cost of the pin

		//return 0 if not found
		pin pin_ptr_of_fd(int fd);
		connection* connection_ptr_of_fd(int fd);
};

#endif

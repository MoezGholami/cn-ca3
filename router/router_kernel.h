#ifndef __siamoz_cnca3_router_kernel
#define __siamoz_cnca3_router_kernel

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include "../connection/connection.h"
#include "../message/message.h"

using namespace std;

const int default_pin_cost	=	default_connection_cost;

typedef string pin;
const pin base_pin="base";//first pin which all routers have
const pin null_pin="";


class router_kernel
{
	public:
		router_kernel(void);
		~router_kernel(void);

		void add_pin(pin p);
		void delete_pin(pin p);
		void change_local_cost(pin p, int new_cost);
		void connect_to_router(pin mine, pine others, int other_port);

		//if returns 0, it means the connection is closed and
		//the fd should be removed from fd_set
		//normally returns something not equal to zero
		int handle_message_of_fd(int fd);

	private:
		vector<pin> pins; //caution: readundent but for easy checking.
		map<pin, int> pin_local_cost;
		map<pin, vector<connection*> > pin_connections;

		void update_pin_cost(pin p);	//talk to peer to get the new cost of the pin

		//return 0 if not found
		pin pin_of_fd(int fd);
		pin pin_of_connection_ptr(connection *con_ptr);
		connection* connection_ptr_of_fd(int fd);

		int handle_new_connection(int new_fd);
		void handle_closing_connection(connection *con_ptr);
};

#endif

#ifndef __siamoz_cnca3_router_kernel
#define __siamoz_cnca3_router_kernel

#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <sstream>
#include "../connection/connection.h"
#include "../message/message.h"
#include "../util/util.h"

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
		void connect_to_router(pin mine, pin others, int other_port);
		void disconnect_pin(pin p);

		//if returns 0, it means the connection is closed and
		//the fd should be removed from fd_set
		//normally returns something not equal to zero
		int handle_message_of_fd(int fd);

		void send_debug_message(pin p);
		void inform_running_port(int p);

		void show_tables(void);//TODO: complete

		//only once
		int get_new_client_like_connection_fd();

	private:
		vector<pin> pins; //caution: readundent but for easy checking.
		map<pin, int> pin_local_cost;
		map<pin, vector<connection*> > pin_connections;

		map<string, connection*> unicast_routing_table;
		map<string, vector<connection *> > multicast_routing_table;

		int debug_message_count;
		int my_running_port;	//used for debugging
		int new_client_like_connection_fd;

		void update_pin_cost(pin p);	//talk to peer to get the new cost of the pin
		void synch_connection_new_local_cost(connection *con_ptr, int cost); //talk to one peer

		//return 0 if not found
		pin pin_of_fd(int fd);
		pin pin_of_connection_ptr(connection *con_ptr);
		connection* connection_ptr_of_fd(int fd);

		int handle_new_connection(int new_fd);
		void handle_closing_connection(connection *con_ptr);
		void delete_corrupted_connections_of_pin(pin p);
		void delete_corrupted_connections_of_all_pins(void);

		void handle_message(connection *con_ptr, message &m);
		void handle_router_message(connection *con_ptr, const message &m);
		void handle_router_cost_message(connection *con_ptr, const message &m);

		void update_tables(connection *con_ptr, const message &m);

		void handle_unicast_message(connection *con_ptr,message &m);

		void broadcast(const message &m, connection *exception=0);
};

#endif

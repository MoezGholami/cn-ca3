#ifndef __siamoz_cnca3_server_core
#define __siamoz_cnca3_server_core

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include "../connection/connection.h"

using namespace std;

class server_core
{
	public:
		server_core(string uip, int rp);
		~server_core(void);

		int get_connection_fd(void);

		void do_stdin_command(string line);
		void handle_connction_message(void);

	private:
		connection *cn;
		string unicast_ip;
		int router_port;

		map<string, string> name2unicast;
		map<string, string> name2multicast;

		void handle_disconnection(void);
		string list_of_groups(void);
};

#endif

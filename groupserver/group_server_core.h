#ifndef __siamoz_cnca3_group_server_core
#define __siamoz_cnca3_group_server_core

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include "../connection/connection.h"
#include "../message/message.h"

using namespace std;

class group_server_core
{
	public:
		group_server_core(string n, string uip, string mip, string sip, int rp);
		~group_server_core(void);

		int get_connection_fd(void);

		void do_stdin_command(string line);
		void handle_connction_message(void);

	private:
		connection *cn;
		string name;
		string unicast_ip;
		string multicast_ip;
		string server_ip;
		int router_port;

		void handle_disconnection(void);
};

#endif

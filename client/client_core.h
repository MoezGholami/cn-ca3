#ifndef __siamoz_cnca3_group_server_core
#define __siamoz_cnca3_group_server_core

#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <map>
#include "../connection/connection.h"
#include "../message/message.h"
#include "../util/util.h"

using namespace std;

const int rejoin_turnaround=1000;

class client_core
{
	public:
		client_core(string n, string uip, string sip, int rp);
		~client_core(void);

		int get_connection_fd(void);

		void do_stdin_command(string line);
		void handle_connction_message(void);

	private:
		connection *cn;
		string name;
		string unicast_ip;
		string server_ip;
		int router_port;
		int last_uni_id_sent;
		int rejoin_counter;
		int last_rejoined;

		map<string, string> gname2unicast;
		map<string, string> gname2multicast;
		string selected_gname;

		void handle_disconnection(void);

		void try2rejoin_someone(void);
		void show_groups(void);
		void send2group(const string &line);
		void select(const string &line);
		void leave(const string &line);
		void start_joining(const string &line);
		void send_sync(void);

		void handle_own_unicast_message(const message &receiving);
		void handle_server_message(const message &receiving);

		void  try2join(const string &gname);
};

#endif

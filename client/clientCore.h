#ifndef __SIAMOZ__clientCore
#define __SIAMOZ__clientCore

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <map>

#include "../connection/connection.h"
#include "../message/message.h"

using namespace std;

class MIP {
	public:
	MIP(string mip,string gn);
	string multicast_IP;
	string group_name;
};

class ClientCoreClerk{
	public:
	ClientCoreClerk(int port_nums,string server_IP,string router_IP,string username,string general_IP);
	int doServerCommand();
	void doClientCommand(int fd);
	private:
	int port;
	connection* cn;
	string router_IP;
	string server_IP;
	string username;
	string general_IP;
	int global_mid;
	vector<string> group_names;
	vector<MIP*> mip_vector;
	string current_group;
	string current_IP;
};


#endif

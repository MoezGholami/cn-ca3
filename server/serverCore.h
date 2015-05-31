#ifndef __SIAMOZ__serverCore
#define __SIAMOZ__serverCore

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


class SIP {
	public:
	string server_IP;
	string multicast_IP;
	string group_name;
	SIP(string sip,string mip,string gn);
};

class ServerCoreClerk{
	public:
	ServerCoreClerk(int port_num,string gip);	
	int doServerCommand();
	void doClientCommand(int fd);
	private:
	vector<SIP*> sip_vector;
	int port;
	connection* cn;
	string general_IP;
	int global_mid;
};


#endif


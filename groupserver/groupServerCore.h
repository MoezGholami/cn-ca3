#ifndef __SIAMOZ__groupServerCore
#define __SIAMOZ__groupServerCore

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

using namespace std;


class GroupServerCoreClerk{
	public:
	GroupServerCoreClerk(int port_num,string gn,string gip,string sip);
	void doClientCommand(int fd);
	int doServerCommand();
	private:
	bool flag_set;
	int global_mid;
	bool flag_add;
	int port;
	int file_descriptory;
	string group_name;
	string general_IP;
	string multicast_IP;
	string server_IP;
	connection * cn;
};


#endif



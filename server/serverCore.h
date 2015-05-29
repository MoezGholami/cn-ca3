#ifndef __SIAMOZ__switchCore
#define __SIAMOZ__switchCore

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
	ServerCoreClerk(int port_num);	
	int doServerCommand();
	private:
	int port;
};


#endif


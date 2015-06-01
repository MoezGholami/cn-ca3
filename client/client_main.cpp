#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include "../connection/connection.h"
#include "client_core.h"

using namespace std;

int main(int argc, char* argv[]);

#define STDIN (0)
#define STDOUT (1)

int main(int argc, char* argv[])
{
	int it_fd, status, router_port;
	struct timeval tv;
	fd_set read_fdset, temp_fdset;
	string stdin_line, name, unicast_ip, server_ip;

	if(argc<5)
	{
		cout<<"too few arguments, "<<"list of requierd arguments:\n";
		cout<<"name\n"<<"my ip\n"<<
			"my router port (immediately connecting to port)\n"<<
			"my server ip (connected on join and show requests)\n";
		return -1;
	}

	name=argv[1];
	unicast_ip=argv[2];
	router_port=atoi(argv[3]);
	server_ip=argv[4];

	client_core core(name, unicast_ip, server_ip, router_port);

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(STDIN, &read_fdset);

	if(core.get_connection_fd()==virgin_fd)
	{
		cout<<"could not connect to port exiting...\n";
		return 1;
	}
	FD_SET(core.get_connection_fd(), &read_fdset);

	/* Wait up to five seconds. */
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;

	while(1)
	{
		memcpy(&temp_fdset, &read_fdset, sizeof(temp_fdset));
		status = select(FD_SETSIZE, &temp_fdset, (fd_set *)0, (fd_set *)0, (struct timeval*) &tv);
		if(status < 1)
		{
			cerr<<"select error\n";
			return -3;
		}

		for(it_fd = 0; it_fd<FD_SETSIZE; it_fd++)
		{
			if(FD_ISSET(it_fd, &temp_fdset))
			{
				if(it_fd == STDIN)
				{
					//read from stdin
					getline(cin, stdin_line);
					core.do_stdin_command(stdin_line);
				}
				else
					core.handle_connction_message();
			}
		}
	}
	return 0;
}

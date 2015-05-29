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
#include "router_shell.h"

using namespace std;

int main(int argc, char* argv[]);
void clear_buff(char *x,size_t s);

const int num_of_connection = 10;
const int buffer_size = 20480;

#define STDIN (0)
#define STDOUT (1)

int main(int argc, char* argv[])
{
	int port_number, server_fd, binding_st, listening_st, ret_val, new_sock_fd, it_fd, status;
	struct sockaddr_in server_addr;
	struct timeval tv;
	string stdin_line;
	router_shell shell;
	if(argc<2)
	{
		cout<<"too few arguments\n"<<"the first argument is the port number\n";
		return -1;
	}
	port_number = atoi(argv[1]);
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_number);

	cout<<"Listening from socket ...\n";

	//binding 
	binding_st = bind(server_fd,(struct sockaddr*) &server_addr, sizeof(server_addr));
	if(binding_st == -1)
	{
		cerr<<"bind error\n";
		return -1;
	}
	//listenning
	listening_st = listen(server_fd, num_of_connection);
	if(listening_st == -1)
	{
		cerr<<"listen error\n";
		return -2;
	}
	fd_set read_fdset, temp_fdset;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDIN, &read_fdset);

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
					shell.do_router_shell_command(stdin_line);
				}
				else if(it_fd == server_fd)
				{	
					new_sock_fd = accept(server_fd, NULL, NULL);
					if(new_sock_fd < 0)
					{
						cerr<<"error on accepting.\n";
						return -4;
					}
					cout<<"new client"<<endl;
					FD_SET(new_sock_fd, &read_fdset);
				}
				else
				{
					int n, m;
					char buff_read [buffer_size], response_buff[buffer_size];
					clear_buff(buff_read, buffer_size);
					clear_buff(response_buff, buffer_size);

					n = read(it_fd, buff_read, buffer_size-1);
					if(n == 0)
					{
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						cout<<"client out"<<endl;
					}
					else if(n < 0)
					{
						cout<<"Error reading"<<endl;
					}

					//after reading successfully
					else
					{
						//after reading client message
						string client_input = buff_read, server_reply;

						cout<<"new query got.\n";
						server_reply=shell.get_response_of_gotten_message(client_input, it_fd);
						int s = write(it_fd, server_reply.c_str(), server_reply.size());
						if(s < 0)
							cout<<"send reply error\n";
					}
				}
			}
		}
	}


	close(server_fd);
	return 0;
}

void clear_buff(char *x,size_t s)
{
	for(int i=0;i<s;i++)
		x[i] = 0;
}

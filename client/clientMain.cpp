//tartib argoman ha
//name
//server ip
//router ip
//router port number
//client ip


////////////////////

#include "clientCore.h"
using namespace std;

#define STDIN 0
#define STDOUT 1
#define STR_SIZE 20480
int main(int argn, char** args)
{
	int port_number;
	//if(argn == 3)
		port_number = atoi(args[4]);
//	else 
//		port_number = 2020; // default
	//string mac = args[2];
	//char *directory_name = args[2];
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_addr;
	string username = args[1];
	string server_IP = args[2];
	string router_IP = args[3];
	string general_IP = args[5];
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr = inet_addr(args[5]);
	server_addr.sin_port = htons(port_number);
	ClientCoreClerk scc(port_number,server_IP,router_IP,username,general_IP);
	string capu;

	char res_buff[STR_SIZE];
	clear_buff(res_buff, STR_SIZE);
	
	cout<<"Listening from socket ...\n";
	//binding 
	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int new_sock_fd, it_fd;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDIN, &read_fdset);

	/* Wait up to five seconds. */
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;

	int status;

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
					int new_sock_s;
					new_sock_s = scc.doServerCommand();	
					if( new_sock_s != -1){
						FD_SET(new_sock_s, &read_fdset);
						cout<<"Added to FD_SET"<<endl;
					}
				}
				else if(it_fd == server_fd)
				{	
					new_sock_fd = accept(server_fd, NULL, NULL);
					//scc.addFileDescriptor(new_sock_fd);
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
	
					//TODO create wrapper
						scc.doClientCommand( it_fd );
					//scc.forwardClientPacket(sendingPacket, it_fd);
					//int s = write(it_fd, (char*)(&sendingPacket), sizeof(Packet));
					//if(s < 0)
					//	cerr<<"send reply error\n";
					/*else if(mystrcmp(buff_read, "DC") == 0)
					{
						write(it_fd, "Disconnecting in Progress ...\n",
								sizeof("Disconnecting in Progress ...\n"));
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						write(STDOUT, "Removing One Client_fd\n", sizeof("Removing One Client_fd\n"));
					}*/
				}
			}
		}
	}


	close(server_fd);
	return 0;
}




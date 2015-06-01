// tartib argoman ha : 
// 1 gourp_name
// 2 server_IP
// 3 general_IP
// 4 port_num

#include "groupServerCore.h"
using namespace std;


#define STDIN 0
#define STDOUT 1
#define STR_SIZE 20480
int main(int argn, char** args)
{
	int port_number;
	string group_name,server_IP,general_IP;
	port_number = atoi(args[4]);
	general_IP = args[3];
	server_IP = args[2];
	group_name = args[1];
	cout<<"Group IP = "<<general_IP<<endl;
	const int num_of_connection = 4;
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	//server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_addr.s_addr = inet_addr(args[3]);
	server_addr.sin_port = htons(port_number);
	GroupServerCoreClerk scc(port_number,group_name,general_IP,server_IP);
	string capu;

	char res_buff[STR_SIZE];
	clear_buff(res_buff, STR_SIZE);
	
	cout<<"Listening from socket ...\n";
	struct sockaddr_in client_addr;
	//binding 
	int binding_st = bind(server_fd,(struct sockaddr*) &server_addr, sizeof(server_addr));
	if(binding_st == -1)
	{
		cerr<<"bind error\n";
		return -1;
	}
	//listenning
	int listening_st = listen(server_fd, num_of_connection);
	if(listening_st == -1)
	{
		cerr<<"listen error\n";
		return -2;
	}
	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int ret_val;
	int new_sock_fd, it_fd;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDIN, &read_fdset);

	/* Wait up to five seconds. */
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;

	unsigned int size_of_client_addr = sizeof(client_addr);

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
					int n, m;
					char buff_read [STR_SIZE], response_buff[STR_SIZE];
					clear_buff(buff_read, STR_SIZE);
					clear_buff(response_buff, STR_SIZE);
					n = read(it_fd,res_buff, STR_SIZE);
					if(n == 0)
					{
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						// TODO remove it from fds .........
						cerr<<"client out"<<endl;
					}
					else if(n < 0)
					{
						cerr<<"Error reading"<<endl;
					}

					//after reading successfully
					else
					{
						// the messages gotten from the clients-routers ...
						
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
	}


	close(server_fd);
	return 0;
}




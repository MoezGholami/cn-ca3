#ifndef __siamoz_cnca3_connection
#define __siamoz_cnca3_connection

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "../message/message.h"

using namespace std;

const int buffer_size = 20480;

const string local_host_ip_address	=	"127.0.0.1";
const int virgin_fd			=	-1;
const int default_connection_cost	=	1;

//yek bar masraf
class connection
{
	public:
		connection(void);
		connection(int fd);
		connection(int server_port_no, const string &server_ip_address);
		~connection(void);

		void close(void);

		// send a message without expecting the result
		// e.g: send response of a question
		void send_message(const message& sending);

		// gets and reads a message.
		// the return value is return value of read
		// returns -2 if the connection is not ready
		int get_message(message& receiving);

		//only for virgin connections
		//returns 1 on success, 0 on error
		int set_fd(int fd);

		//do not close it, please
		int get_fd(void);

		bool is_virgin(void);
		bool is_corrupted(void);
		bool is_closed(void);
		bool is_ok_to_communicate(void);

		int get_cost(void);
		void set_cost(int c);

	private:
		int connection_fd;
		bool closed;
		bool corrupted; //close as you set this
		int cost;

		void set_true_corrupted(void);
		//we do not allow copy, and promise not to use them
		connection(const connection &c);
		connection& operator=(const connection &c);
};

#endif

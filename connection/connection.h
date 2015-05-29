#ifndef __siamoz_cnca3_connection
#define __siamoz_cnca3_connection

#include <string>
#include <unistd.h>

using namespace std;

//TODO: what is connection???
//constructor? given from (as server)???
//proper destructor
//transparent of bidirectional asynchronous q&a connection
//state of being not established / corrupted

const int virgin_fd	=	-1;

//yek bar masraf
class connection
{
	public:
		connection(void);
		connection(int server_port_no, const string &server_ip_address="127.0.0.1");
		~connection(void);

		void close(void);

		//blocking method
		//returns "" on failure
		//check is_ok_to_communicate before and after using it
		string send_q_get_a(const string &sending);

		//TODO: a func ptr to get response from user of connection.
		//compelete this after writing main

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

		//we do not allow copy, and promise not to use them
		connection(const connection &c);
		connection& operator=(const connection &c);
};

#endif

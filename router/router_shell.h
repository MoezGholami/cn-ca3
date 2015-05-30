#ifndef __siamoz_cnca3_router_shell
#define __siamoz_cnca3_router_shell

#include <string>
#include <sstream>
#include "router_kernel.h"
#include "../connection/connection.h"

using namespace std;

class router_shell
{
	public:
		router_shell(void);

		void do_router_shell_command(const string &command);

		//if returns 0, it means the connection is closed and
		//the fd should be removed from fd_set
		//normally returns something not equal to zero
		int handle_message_of_fd(int fd);

		int get_new_client_like_connection_fd();

		void inform_running_port(int p);//only for debugging

	private:
		router_kernel kernel;

		int new_client_like_connection_fd;
};

#endif

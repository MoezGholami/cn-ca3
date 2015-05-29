#ifndef __siamoz_cnca3_router_shell
#define __siamoz_cnca3_router_shell

#include <string>
#include "router_kernel.h"

using namespace std;

class router_shell
{
	public:
		void do_router_shell_command(const string &command);
		string get_response_of_gotten_message(const string &message, int receiver_fd);

	private:
		router_kernel kernel;
};

#endif

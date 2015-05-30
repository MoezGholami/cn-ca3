#include "router_shell.h"

router_shell::router_shell(void)
{
	new_client_like_connection_fd=virgin_fd;
}

void router_shell::do_router_shell_command(const string &command)
{
	stringstream ss;
	string parse1, parse2;
	int parse_int;

	ss<<command;
	ss>>parse1;

	if(parse1=="Eth")
	{
		ss>>parse2;
		kernel.add_pin((pin)parse2);
	}
	else if(parse1=="NoEth")
	{
		ss>>parse2;
		kernel.delete_pin((pin)parse2);
	}
	else if(parse1=="Connect")
	{
		ss>>parse1>>parse2>>parse_int;
		kernel.connect_to_router(parse1, parse2, parse_int);
	}
	else
	{
	}
	//TODO: complete
}

int router_shell::get_new_client_like_connection_fd()
{
	int result=new_client_like_connection_fd;
	new_client_like_connection_fd=virgin_fd;
	return result;
}

int router_shell::handle_message_of_fd(int fd)
{
	return kernel.handle_message_of_fd(fd);
}

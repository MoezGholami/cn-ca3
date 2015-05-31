#include "router_shell.h"

router_shell::router_shell(void)
{
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
	else if(parse1=="Disconnect")
	{
		ss>>parse2;
		kernel.disconnect_pin(parse2);
	}
	else if(parse1=="debug")
	{
		ss>>parse2;
		kernel.send_debug_message(parse2);
	}
	else if(parse1=="ChangeCost")
	{
		ss>>parse1>>parse_int;
		kernel.change_local_cost(parse1, parse_int);
	}
	else if(parse1=="Show")
	{
		kernel.show_tables();
	}
	else
	{
		cout<<"command not recognized.\n";
	}
}

int router_shell::get_new_client_like_connection_fd()
{
	return kernel.get_new_client_like_connection_fd();
}

int router_shell::handle_message_of_fd(int fd)
{
	return kernel.handle_message_of_fd(fd);
}

void router_shell::inform_running_port(int p)
{
	kernel.inform_running_port(p);
}

#include "router_shell.h"

router_shell::router_shell(void)
{
	new_client_like_connection_fd=virgin_fd;
}

void router_shell::do_router_shell_command(const string &command)
{
}

int get_new_client_like_connection_fd()
{
	int result=new_client_like_connection_fd;
	new_client_like_connection_fd=virgin_fd;
	return result;
}

int router_shell::handle_message_of_fd(int fd)
{
	return kernel.handle_message_of_fd(fd);
}

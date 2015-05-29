#include "connection.h"

connection::connection(void)
{
	connection_fd=virgin_fd;
	corrupted=false;
	closed=false;
}

connection::connection(int server_port_no, const string &server_ip_address)
{
	struct sockaddr_in serv_addr;
	int status;

	closed=false;
	corrupted=true;

	clear_buff(input_buffer, STR_SIZE);
	connection_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(server_ip_address.c_str());
	serv_addr.sin_port = htons(server_port_no);
	status = connect(connection_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	if(status < 0)
		cout<<"Conention failed\n";
	else
	{
		cout<<"Successfully Connected\n";
		corrupted=false;
	}
}

connection::~connection(void)
{
	close();
}

void connection::close(void)
{
	if(closed)
		return ;
	::close(connection_fd);
	closed=true;
}

string connection::send_q_get_a(const string &sending)
{
	if(!is_ok_to_communicate())
		return "";
	//TODO: complete this and add return value
}

int connection::set_fd(int fd)
{
	if(!is_virgin() || is_closed() || is_corrupted())
		return 0;
	connection_fd=fd;
	return 1;
}

int connection::get_fd(void)
{
	return connection_fd;
}

bool connection::is_virgin(void)
{
	return connection_fd==virgin_fd;
}

bool connection::is_corrupted(void)
{
	return corrupted;
}

bool connection::is_closed(void)
{
	return closed;
}

bool connection::is_ok_to_communicate(void)
{
	return !is_virgin() && !is_corrupted() && !is_closed();
}

int connection::get_cost(void)
{
	return cost;
}

void connection::set_cost(int c)
{
	cost=c;
}

connection::connection(const connection &c)
{
	connection_fd=virgin_fd;
	corrupted=true;
	closed=true;
}

connection& connection::operator=(const connection &c)
{
	return *this;
}

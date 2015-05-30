#include "connection.h"

connection::connection(void)
{
	connection_fd=virgin_fd;
	corrupted=false;
	closed=false;
	set_cost(default_connection_cost);
}

connection::connection(int fd)
{
	connection_fd=virgin_fd;
	corrupted=false;
	closed=false;
	set_fd(fd);
	set_cost(default_connection_cost);
}

connection::connection(int server_port_no, const string &server_ip_address)
{
	struct sockaddr_in serv_addr;
	int status;

	set_cost(default_connection_cost);

	closed=false;
	corrupted=true;

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

message connection::send_q_get_a(const message &q)
{
	int n;
	message result;

	if(!is_ok_to_communicate())
		return null_message;


	send_message(q);
	if(!is_ok_to_communicate())
		return null_message;

	n=get_message(result);

	if(n==0)
		cout<<"in send_q_get_a: extranous connection close on fd: "<<connection_fd<<endl;

	if(!is_ok_to_communicate())
		return null_message;

	return result;
}

void connection::send_message(const message& sending)
{
	int s;
	string sending_str;

	if(!is_ok_to_communicate())
		return ;

	sending_str=string_from_message(sending);

	s = write(connection_fd, sending_str.c_str(), sending_str.size());
	if(s < 0)
	{
		cout<<"connection with fd: "<<connection_fd<<
			", send error, corrupted.\n";
		set_true_corrupted();
		return ;
	}
}

int connection::get_message(message& receiving)
{
	int n;
	char buff_read [buffer_size];
	string receiving_str;

	if(!is_ok_to_communicate())
		return -2;


	clear_buff(buff_read, buffer_size);
	n = read(connection_fd, buff_read, buffer_size-1);
	if(n == 0)
	{
		cout<<"connection close on fd: "<<connection_fd<<endl;
		close();
		return n;
	}
	else if(n < 0)
	{
		cout<<"connection with fd: "<<connection_fd
			<<"error reading, corrupted."<<endl;
		set_true_corrupted();
		return n;
	}
	receiving_str=buff_read;
	receiving=message_from_string(receiving_str);
	return n;
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

void connection::set_true_corrupted(void)
{
		corrupted=true;
		close();
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

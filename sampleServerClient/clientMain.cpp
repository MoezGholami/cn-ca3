#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

#define STR_SIZE 2048

void clear_buff(char *x,size_t s){
	for(int i=0;i<s;i++){
		x[i] = 0;
	}
}

int main(int argn, char** args){

	char input_buffer[STR_SIZE];
	clear_buff(input_buffer, STR_SIZE);
	string inputComm;

	int temp;

	while(cin>>inputComm)
	{
		//parsing input_buffer
		if(inputComm == "Connect")// don't forget to check reconnect!!
		{
			cin>>inputComm;
			if(inputComm!="Server")
				continue;
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			cin>>temp;
			getline(cin,inputComm);
			int port_no = temp;
			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(port_no);
			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			
			if(status < 0)
			{
				cerr<<"Conention failed\n";
				continue;
			}
			else cerr<<"Successfully Connected\n";
			while(1)
			{
				//read command
				getline(cin,inputComm);

				//send command for server
				int bytes_written = write(fd, inputComm.c_str(), inputComm.size());
				cerr<<"command :\n"+inputComm+"\n sent.\n\n";
				if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
				}
				string reply;
				//get response from server
				char res_buff[STR_SIZE];
				clear_buff(res_buff, STR_SIZE);
				int read_status = read(fd, res_buff, STR_SIZE);
				reply = res_buff;
				//print server reply
				cout<<"server reply: "<<reply<<endl;
			}
			close(fd);
		} else {
			cout<<"A connection must be established\n";
		}
	}

	//free allocated memories
	return 0;
}

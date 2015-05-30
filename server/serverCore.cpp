#include "serverCore.h"


SIP::SIP(string sip,string mip,string gn){
	server_IP = sip;
	multicast_IP = mip;
	group_name = gn;
}

ServerCoreClerk::ServerCoreClerk(int port_num){
	port = port_num;
}

void ServerCoreClerk::doClientCommand( int fd ){
	message mssg;
	int n,status;
	n = read( fd, (char*)(&mssg), sizeof(mssg));
	if( n<0 ){
		cout<<"reading failed ..."<<endl;
	}
	status = mssg.type;
	if(status == my_ip_intro_type ){
		
	}
	s = write( fd, (char*)(&mssg), sizeof(mssg));
	
}

int ServerCoreClerk::doServerCommand(){
	string comm1,comm2;
	cin>>comm1;
	int portNu;
	if(comm1 == "Connect"){
		cin>>comm2;
		if(comm2 == "Router"){
			int n, m;//return value of read/write calls
			string temp;
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			cin>>temp;
			string port_no = temp;
			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(atoi(port_no.c_str()));
			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			//string chert;
			//getline(cin, chert);
			
			if(status < 0)
			{
				cerr<<"Conention failed\n";
			}
			else cerr<<"Successfully Connected\n";
			return fd;			
		}
	}
	return -1;
}

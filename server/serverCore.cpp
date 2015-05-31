#include "serverCore.h"

SIP::SIP(string sip,string mip,string gn){
	server_IP = sip;
	multicast_IP = mip;
	group_name = gn;
}

ServerCoreClerk::ServerCoreClerk(int port_num,string gip){
	port = port_num;
	global_mid = 0;
	general_IP = gip;
}

void ServerCoreClerk::doClientCommand( int fd ){
	cout<<"clientcommand"<<endl;
	message mssg;
	stringstream ss;
	string body,t1,t2,t3,t4;
	int status;
	if( !cn->is_ok_to_communicate() ){
		cout<<"Connection Failed ..."<<endl;
		return ;
	}
	cn->get_message(mssg);
	
	status = mssg.type;
	body = mssg.body;
	if(status == multicast_ip_intro_type ){
		cout<<"multicast::"<<mssg.body<<endl;
		global_mid = mssg.id + 1;
		message reply(mssg.destination_ip,mssg.source_ip,mssg.id, multicast_ip_intro_type , 10 ,"Group Information was regisered.");
		ss << body;
		ss >> t1;
		ss >> t2;
		ss >> t3;
		sip_vector.push_back(new SIP(t2,t3,t1) );
		cn->send_message(reply);
	} else if( status == log_type ){
		cout<<body<<endl;
	} else {
		cout<<"No default type found:"<<body<<endl;
	}
	//	s = write( fd, (char*)(&mssg), sizeof(mssg));
	
}

int ServerCoreClerk::doServerCommand(){
	string comm1,comm2;
	cin>>comm1;
	int portNu;
	if(comm1 == "Connect"){
		cin>>comm2;
		if(comm2 == "Router"){
			int n, m;//return value of read/write calls
			int temp;
			cin >> temp;
			cn = new connection(temp, local_host_ip_address );
			if(cn->is_ok_to_communicate())
			{
				cout<<"connected successfuly"<<endl;
				//sending some protocolic packet
				message mx(general_IP,"ks",global_mid,2,10,"");
				global_mid++;
				cn->send_message(mx);
				return cn->get_fd();
			}else{
			 	cout<<"Connection was down ..."<<endl;      
				return -1;
			}
		}
	}
	return -1;
}

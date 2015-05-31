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
	} else if( status == unicast_message_type ) {
		ss << body;
		ss >> t1;
		if( t1 == "Get" ){
			ss >> t2;
			ss >> t3;
			if( t2 == "group" && t3 == "list" ){
				string ggl="";
				for(int i=0;i<sip_vector.size();i++){
					ggl+=sip_vector[i]->group_name+" <-> "+sip_vector[i]->multicast_IP+"\n";
				}
				message reply(general_IP,mssg.source_ip,global_mid,unicast_message_type,10,ggl);
				global_mid++;
				cn->send_message(reply);
			}
		} else if( t1 == "Join"){
			cout<<"Request for join ..."<<endl;
			ss >> t2;
			cout<<"Pending..."<<endl;
			for(int i=0;i<sip_vector.size();i++){
				if(sip_vector[i]-> group_name == t2){
					cout<<"Group was found ..."<<endl;
					message reply(general_IP,mssg.source_ip,global_mid,unicast_message_type,10,sip_vector[i]->multicast_IP);
					global_mid++;
					cn->send_message(reply);
					break;
				}
				if(i == sip_vector.size()-1){
					cout<<"Group was not found ..."<<endl;
					message reply(general_IP,mssg.source_ip,global_mid,unicast_message_type,10,"404");
					global_mid++;
					cn->send_message(reply);
				}
			}
		}
	}else {
		cout<<"No default type found:"<<status<<"&&"<<body<<endl;
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

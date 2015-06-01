// faghat unicast o multicast

// ssssssssss

#include "groupServerCore.h"

GroupServerCoreClerk::GroupServerCoreClerk(int port_num,string gn,string gip,string sip){
	flag_set = false;
	flag_add = false;
	port = port_num;
	group_name = gn;
	general_IP = gip;
	server_IP = sip;
	global_mid = 0;
}

void GroupServerCoreClerk::doClientCommand(int fd){
	message mssg;
	stringstream ss;
	string body,t1;
	if( cn->is_ok_to_communicate() ){
		cn->get_message(mssg);
	} else {
		cout<<"Message did not send"<<endl;
		return ;
	}
	body = mssg.body;
	if( mssg.type == log_type ){
		cout<<mssg.body<<endl;	
	} else if( mssg.type == unicast_message_type ){
		ss << body;
		ss >> t1;
		if( t1 == "synch"){
			// fbs
		} else {
			message reply(general_IP,multicast_IP,global_mid,multicast_message_type,10,mssg.body);
			global_mid++;
			if(cn->is_ok_to_communicate()){
				cout<<"Multicasting massage:"<<mssg.body<<endl;
				cn->send_message(reply);
			}
			else 
				cout<<"Failed to multicast message."<<endl;
		}
	}
}

int GroupServerCoreClerk::doServerCommand(){
	string comm1,comm2;
	cin>>comm1;
	if(comm1 == "Connect"){
		cin>>comm2;
		if(comm2 == "Router"){
			int temp;
			cin>>temp;
			if(!flag_set){
				cout<<"General IP must be set."<<endl;
				return -1;
			}
			cn = new connection(temp, local_host_ip_address );
			if( cn->is_ok_to_communicate() ){
				cout<<"connected successfuly"<<endl;
				flag_add = true;
				file_descriptory = cn->get_fd();
				//sending some protocolic packet
				message mx(general_IP,"ks",global_mid,2,10,"");
				cn->send_message(mx);
				global_mid++;
				return file_descriptory;
			} else {
				cout<<"Connection failed."<<endl;
				return -1;
			}
			//string chert;
			//getline(cin, chert);
		}
	} else if( comm1 == "Set"){
		cin >> comm1>> comm2;
		if( comm1 == "IP" && comm2 == "multicast" ){
			cin >> comm1;
			multicast_IP = comm1;
		}
		flag_set = true;
	} else if( comm1 == "Add"){
		cin>> comm2;
		if( comm2 == "server"){
			if(!flag_set){
				cout<<"Connection must be established."<<endl;
				return -1;
			}
		}
		message mssg(general_IP,server_IP,global_mid, multicast_ip_intro_type, 10, group_name+" "+general_IP+" "+multicast_IP);
		global_mid ++;
		if( cn->is_ok_to_communicate() )
			cn->send_message(mssg);
		else 
			cout<<"Packets did not send successfully"<<endl;

	}
	return -1;
}


#include "clientCore.h"

MIP::MIP(string mip,string gn){
	multicast_IP = mip;
	group_name = gn;
}

ClientCoreClerk::ClientCoreClerk(int port_num,string sip,string rip,string un,string gip){
	port = port_num;
	global_mid = 0;
	server_IP = sip;
	router_IP = rip;
	general_IP = gip;
	username = un;
	current_group = "undefined";
}

void ClientCoreClerk::doClientCommand( int fd ){
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
	if( status == log_type){
		cout<<body<<endl;
	} else if( status == multicast_message_type){
		cout<<"Was multicasted: "<<body<<endl;	
	} else if( status == unicast_message_type ){
		cout<<"Was unicasted: "<<body<<endl;	
	}
}

int ClientCoreClerk::doServerCommand(){
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
				message mx(general_IP,"ks",global_mid,my_ip_intro_type,10,"");
				global_mid++;
				cn->send_message(mx);
				return cn->get_fd();
			}else{
			 	cout<<"Connection was down ..."<<endl;      
				return -1;
			}
		}
	} else if(comm1 == "Get"){        /////////////////////////////////// CHECK CONNECTION FIRSTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
		cin>>comm2>>comm1;
		if(comm2 == "group" && comm1 == "list"){
			if( cn->is_ok_to_communicate() ){
				message mx(general_IP,server_IP,global_mid,unicast_message_type,10,"Get group list");
				global_mid++;
				cn->send_message(mx);
			} else {
			 	cout<<"Connection was down ..."<<endl;      
				return -1;
			}
		}
	} else if(comm1 == "Join"){
		cin>>comm2;
		if( cn->is_ok_to_communicate() ){
			message mx(general_IP,server_IP,global_mid,unicast_message_type,10,"Join "+comm2);
			message reply,reply2;
			global_mid++;
			cn->send_message(mx);
			cn->get_message(reply);
			if( reply.body == "404" ){
				cout<<"Group does not exist."<<endl;
				return -1;
			}
			global_mid = reply.id + 1;
			cout<<"Group was found ..."<<endl;
			message rx(general_IP,server_IP,global_mid,membership_report_type,10,reply.body);
			global_mid++;
			cn->send_message(rx);
			cerr<<"OS"<<endl;
			cn->get_message(reply2);
			cerr<<"X"<<endl;
			while( reply2.type != membership_is_member_type ){
				cn->get_message(reply2);
			}
			global_mid = reply2.id +1;
			message rx2(general_IP,server_IP,global_mid,membership_i_join_u_type,10,reply.body);
			global_mid ++;

		} else {
		 	cout<<"Connection was down ..."<<endl;      
			return -1;
		}
	} else if(comm1 == "Leave"){
		cin >> comm2;
		for(int i=0;i<mip_vector.size();i++){
			if(mip_vector[i]->group_name == comm2){
				message mx(general_IP,server_IP,global_mid,membership_leave_type,10,mip_vector[i]->multicast_IP);
				global_mid++;
				cn->send_message(mx);
				
			}
			if(i==mip_vector.size()-1){
				cout<<"You are not currently joined in this group."<<endl;
			}
		}
	} else if(comm1 == "Select"){
		cin >>comm2;
		for(int i=0;i<mip_vector.size();i++){
			if(mip_vector[i]->group_name == comm2){	
				current_group = comm2;
				current_IP = mip_vector[i]->multicast_IP;
				cout<<"Selection successful."<<endl;
				return -1;
			}
		}
		cout<<"Selection failed."<<endl;
	} else if(comm1 == "Send"){
		cin>>comm2;
		if( current_group == "undefined"){
			cout<<"Please select a group first."<<endl;
			return -1;
		}
		if(comm2 == "file"){
			cin>>comm2;
			if( FileExist(comm2) ){
				string text = wholeAsciiFile( comm2 );
				message mssg(general_IP,server_IP,global_mid,unicast_message_type,10,text);
				global_mid++;
				cn->send_message(mssg);
			} else {
				cout<<"File does not exist ... so stfu if u dont have anything to send."<<endl;
			}
		} else if(comm2 == "message"){
			string mess;
			getline(cin,mess);
			message mssg(general_IP,server_IP,global_mid,unicast_message_type,10,mess);
			global_mid++;
			cn->send_message(mssg);
		}
	} else if( comm1 == "Show"){
		cin>>comm2;
		if(comm2 == "group"){
			for(int i=0;i<mip_vector.size();i++){
				cout<<"Gourp: "<<mip_vector[i]->group_name<<" <-> "<<mip_vector[i]->multicast_IP<<endl;
			}
		}
	}
	return -1;
}


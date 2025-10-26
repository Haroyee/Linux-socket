#include"socket.h"
#include<iostream>
#include<string>

int main(){
	using namespace network::socket;

	Socket c_socket;
	bool flag =  c_socket.connect("120.55.4.228",8888);
	if(flag){
		std::string sbuff;
		
	while(!sbuff.compare("0")){
		sbuff.clear();
		std::cin>>sbuff;
		c_socket.send(sbuff.c_str(),sbuff.size());

		char recvBuff[1024];
		c_socket.recv(recvBuff,sizeof(recvBuff));
		std::cout<<recvBuff<<std::endl;
		
	}
	
	}
	c_socket.close();
	return 0;
}


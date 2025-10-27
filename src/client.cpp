#include"socket.h"
#include<iostream>
#include<string>

int main(){
	using namespace network::socket;

	Socket c_socket;
	bool flag =  c_socket.connect("127.0.0.1",8889);
	if(flag){
		std::string sbuff;
		
	while(sbuff.compare("0")){
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


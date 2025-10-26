#include"socket.h"
#include<iostream>

int main(){
	
	using namespace network::socket;
	Socket s_socket;
	s_socket.bind("127.0.0.1",8888);
	
	s_socket.listen(1024);

	while(true){
	
		int connfd = s_socket.accept();
		Socket c_socket(connfd);
		char recvBuff[1024];
		if(c_socket.recv(recvBuff,sizeof(recvBuff))){
			std::cout<<"client:"<<recvBuff<<std::endl;
			std::string sstr = "ok!";
			c_socket.send(sstr.c_str(),sstr.size());
		}
	}

	std::cout<<"server ps"<<std::endl;

	return 0;
}

#include"socket.h"
#include<iostream>

int main(){
	
	using namespace network::socket;
	Socket s_socket;
	if(!s_socket.bind("127.0.0.1",8889)) return 0;
	
	if(!s_socket.listen(1024)) return 0;

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



	return 0;
}

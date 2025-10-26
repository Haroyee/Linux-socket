#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>       // socket()、bind()等核心函数
#include <netinet/in.h>       // sockaddr_in地址结构
#include <arpa/inet.h>        // inet_pton()、htons()等转换函数
#include <unistd.h>           // close()、read()、write()
#include <string>

namespace network{
	namespace socket{
class Socket{
	private:
		int m_sockfd;
		struct sockaddr_in m_addr;
	public:
		Socket();

		Socket(int );

		~Socket();

		bool bind(const std::string &,int);

		bool connect(const std::string &,int);

		bool listen(int);

		int accept();

		int send(const char *,int);

		int recv(char *,int);

		void close();



};
}
}

#endif

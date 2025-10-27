#include "socket.h"
#include <iostream>
#include <cstring>

int main()
{

	Socket s_socket;
	if (!s_socket.Bind("0.0.0.0", 8889))
		return 0;

	if (!s_socket.Listen(1024))
		return 0;

	int connfd = s_socket.Accept();

	if (connfd > 0)
	{

		Socket c_socket(connfd);

		while (true)
		{
			char rbuf[1024];
			memset(rbuf, 0, sizeof(rbuf));
			if (c_socket.Recv(rbuf, sizeof(rbuf)))
			{
				std::cout << "client: " << rbuf << std::endl;
				std::string sbuf = "ok";
				c_socket.Send(sbuf.c_str(), sbuf.size());
			}
		}
	}
	return 0;
}

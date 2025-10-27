#include "socket.h"
#include <iostream>
#include <cstring>

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    Socket s_socket(listenfd);

    if (!s_socket.Bind("0.0.0.0", 8889))
        return 0;

    if (!s_socket.Listen(1024))
        return 0;

    fd_set read_fds;       // 关注的可读事件集合
    fd_set all_fds;        // 保存所有需监控的 fd（避免每次重新添加）
    int max_fd = listenfd; // 记录最大 fd,当前为listenfd（供 select 的 nfds 参数)

    FD_ZERO(&all_fds);

    FD_SET(listenfd, &all_fds); // 先添加监听 fd

    while (1)
    {
        // 每次调用 select 前需重置 read_fds（因为 select 会修改集合）
        read_fds = all_fds;

        // 阻塞等待可读事件（无超时）
        int nfds = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (nfds == -1)
        {
            perror("select");
            exit(1);
        }

        // 遍历所有 fd，检查哪些就绪
        for (int fd = 0; fd <= max_fd; fd++)
        {
            if (!FD_ISSET(fd, &read_fds))
            {
                continue;
            }

            if (fd == listenfd)
            { // 监听 fd 就绪：新连接
                int client_fd = s_socket.Accept();
                if (client_fd == -1)
                {
                    perror("accept");
                    continue;
                }

                // 将新客户端 fd 添加到监控集合
                FD_SET(client_fd, &all_fds);
                if (client_fd > max_fd)
                {
                    max_fd = client_fd;
                } // 更新最大 fd
                printf("new client: %d\n", client_fd);
            }
            else
            { // 客户端 fd 就绪：有数据可读
                char buf[1024];
                ssize_t n = read(fd, buf, sizeof(buf) - 1);
                if (n <= 0)
                { // 客户端断开或错误
                    printf("client %d disconnected\n", fd);
                    FD_CLR(fd, &all_fds); // 从集合中移除
                    close(fd);
                }
                else
                { // 处理数据
                    buf[n] = '\0';
                    printf("client %d: %s\n", fd, buf);
                    // 回复客户端
                    const char *resp = "ok";
                    write(fd, resp, strlen(resp));
                }
            }
        }
    }

    return 0;
}

#include "socket.h"
#include <iostream>
#include <cstring>
#include <poll.h>

#define MAX_CONN 1024 // 可自定义（不受固定上限限制）

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    Socket s_socket(listenfd);

    if (!s_socket.Bind("0.0.0.0", 8889))
        return 0;

    if (!s_socket.Listen(1024))
        return 0;

    // 初始化 pollfd 数组（第一个元素为监听 fd）
    struct pollfd fds[MAX_CONN];

    for (int ii = 0; ii < MAX_CONN; ii++)
        fds[ii].fd = -1;

    fds[0].fd = listenfd;
    fds[0].events = POLLIN; // 关注可读事件
    int nfds = 1;           // 当前数组中有效元素数量

    while (1)
    {
        // 阻塞等待事件（无超时）
        int ready = poll(fds, nfds, -1);
        if (ready == -1)
        {
            perror("poll");
            exit(1);
        }

        // 遍历所有 fd，检查哪些就绪
        for (int i = 0; i <= nfds; i++)
        {
            if (!(fds[i].revents & POLLIN))
            {
                continue;
            } // 非可读事件跳过

            if (fds[i].fd == listenfd)
            { // 监听 fd 就绪：新连接

                int client_fd = s_socket.Accept();
                if (client_fd == -1)
                {
                    perror("accept");
                    continue;
                }

                // 检查连接数是否超过上限
                if (nfds >= MAX_CONN)
                {
                    printf("too many connections, close %d\n", client_fd);
                    close(client_fd);
                    continue;
                }

                // 添加新客户端 fd 到 pollfd 数组
                fds[nfds].fd = client_fd;
                fds[nfds].events = POLLIN; // 关注可读事件
                nfds++;
                printf("new client: %d (total: %d)\n", client_fd, nfds - 1);
            }
            else
            { // 客户端 fd 就绪：有数据可读
                int client_fd = fds[i].fd;
                char buf[1024];
                ssize_t n = read(client_fd, buf, sizeof(buf) - 1);
                if (n <= 0)
                { // 客户端断开或错误
                  // 移除该 fd（用最后一个元素覆盖，减少数组移动）
                    fds[i] = fds[nfds - 1];
                    nfds--;
                    i--; // 回退索引，避免跳过元素
                    close(client_fd);
                }
                else
                { // 处理数据
                    buf[n] = '\0';
                    printf("client %d: %s\n", client_fd, buf);
                    // 回复客户端
                    const char *resp = "ok";
                    write(client_fd, resp, strlen(resp));
                }
            }
        }
    }

    return 0;
}

#include "socket.h"
#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#define MAX_EVENTS 1024

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    Socket s_socket(listenfd);

    if (!s_socket.Bind("0.0.0.0", 8889))
        return 0;

    if (!s_socket.Listen(1024))
        return 0;

    // 4. 创建 epoll 实例
    int epfd = epoll_create(1); // 参数忽略
    if (epfd == -1)
    {
        perror("epoll_create");
        exit(1);
    }

    // 5. 向 epoll 注册监听 fd（关注可读事件，LT 模式默认）
    struct epoll_event ev;
    ev.events = EPOLLIN; // 水平触发：有数据就通知
    ev.data.fd = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
    {
        perror("epoll_ctl add listen_fd");
        exit(1);
    }
    struct epoll_event events[MAX_EVENTS]; // 存放就绪事件

    while (1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            exit(1);
        }

        // 处理就绪fd
        for (int i = 0; i <= nfds; i++)
        {
            int fd = events[i].data.fd;
            if (fd == listenfd)
            { // 监听 fd 就绪：新连接
                int client_fd = s_socket.Accept();
                if (client_fd == -1)
                {
                    perror("accept");
                    continue;
                }

                // 将新客户端 fd 添加到监控集合
                struct epoll_event new_ev;
                new_ev.events = EPOLLIN; // 水平触发：有数据就通知
                new_ev.data.fd = client_fd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &new_ev) == -1)
                {
                    perror("epoll_ctl add client_fd");
                    close(client_fd);
                }
                printf("new client: %d\n", client_fd);
            }
            else
            { // 客户端 fd 就绪：有数据可读
                char buf[1024];
                ssize_t n = read(fd, buf, sizeof(buf) - 1);
                if (n <= 0)
                { // 客户端断开或错误
                    printf("client %d disconnected\n", i);
                    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, NULL);
                    close(fd);
                }
                else
                { //  处理数据（LT 模式可分多次读）
                    buf[n] = '\0';
                    printf("client %d: %s\n", i, buf);
                    // 回复客户端
                    const char *resp = "ok";
                    write(i, resp, strlen(resp));
                }
            }
        }
    }

    return 0;
}

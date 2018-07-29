
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/epoll.h>

#define BACKLOG 100

int main()
{
    int iListenSock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    inet_aton("0.0.0.0", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);

    int iOpt = 1;
    setsockopt(iListenSock, SOL_SOCKET, SO_REUSEADDR, &iOpt, sizeof(iOpt)); // 标配
    bind(iListenSock, (sockaddr *) &addr, sizeof(addr));
    listen(iListenSock, BACKLOG);

    epoll_event ev;
    ev.data.fd = iListenSock;
    ev.events = EPOLLIN;

    epoll_event events[BACKLOG + 1];

    int epollFD = epoll_create(BACKLOG + 1);   // 告诉内核监测的数目, 返回的epollFD为epoll管理句柄
    epoll_ctl(epollFD, EPOLL_CTL_ADD, iListenSock, &ev);  // 将ev和对应的iListenSock添加到epoll句柄，用于被epollFD管理
    while (1)
    {
        int timeoutMS = - 1; // 永不超时
        int nfds = epoll_wait(epollFD, events, BACKLOG + 1, timeoutMS);   // events和nfds是一对输出值
        printf("nfds is %d\n", nfds);

        for (int i = 0; i < nfds; i ++)
        {
            if (events[i].data.fd == iListenSock)         // 用于监听客户端连接的socket
            {
                int iConnSock = accept(iListenSock, NULL, NULL);
                if (iConnSock < 0)
                {
                    continue;
                }

                ev.data.fd = iConnSock;
                ev.events = EPOLLIN;
                epoll_ctl(epollFD, EPOLL_CTL_ADD, iConnSock, &ev);  // 将ev和对应的iConnSock添加到epoll句柄，用于被epollFD管理

                printf("new sock came, fd is %d\n", iConnSock);
            } else
            {
                int iConnSock = events[i].data.fd;      // 用于通信的socket
                char szBuf[1024] = {0};
                int recvLen = recv(iConnSock, szBuf, sizeof(szBuf) - 1, 0);
                if (recvLen > 0)
                {
                    printf("recv data [%s] from fd [%d]\n", szBuf, iConnSock);
                } else if (0 == recvLen)
                {
                    ev.data.fd = iConnSock;
                    epoll_ctl(epollFD, EPOLL_CTL_DEL, iConnSock, &ev);
                    close(iConnSock);
                    printf("connection closed, local fd is [%d]\n", iConnSock);
                } else
                {
                    ev.data.fd = iConnSock;
                    epoll_ctl(epollFD, EPOLL_CTL_DEL, iConnSock, &ev);
                    close(iConnSock);
                    printf("recv error, local fd is [%d]\n", iConnSock);
                }
            }
        }
    }

    close(epollFD);
    close(iListenSock);
    return 0;
} 
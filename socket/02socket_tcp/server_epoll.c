#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_EVENTS 10

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: <Server> <Port>\n");
        return -1;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd,(struct sockaddr*)&addr, sizeof(addr))) {
        perror("bind");
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        perror("listen");
        return -1;
    }

    int epollfd = epoll_create1(0);
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl: sockfd");
        return -1;
    }
    printf("sockfd = %d, epollfd = %d\n", sockfd, epollfd);

    char buf[256];
    int enable_reuse = 1;
    while (1) {
        int ready = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (ready == -1) {
            perror("epoll_wait");
            return -1;
        }

        for (int i = 0; i < ready; ++i) {
            if (events[i].data.fd == sockfd) {
                // 有新连接
                struct sockaddr_in client_addr;
                socklen_t addrlen;
                int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
                if (client_fd == -1) {
                    perror("accept");
                    return -1;
                }
                printf("new connection: %d\n", client_fd);
                // 将新连接的fd加入监听
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev)==-1) {
                    perror("epoll_ctl: conn");
                    return -1;
                }
            }
            else {
                // 有就绪的fd
                int fd = events[i].data.fd;
                int n = recv(fd, buf, sizeof(buf), 0);
                if (n == -1) {
                    perror("recv");
                    return -1;
                }
                buf[n] = '\0';
                printf("fd = %d, [%d]<<<<<<: %s\n", fd, n, buf);
                if (!strcmp(buf, "quit")) {
                    // 移除监控
                    if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
                        perror("epoll_ctl: del");
                        return -1;
                    }
                    close(events[i].data.fd);
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
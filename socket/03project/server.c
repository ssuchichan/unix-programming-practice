#include "head.h"


void server_epoll(char* argv[]) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr, sizeof(server_addr));
    bzero(&client_addr, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server_addr.sin_port = htons(atoi(argv[1]));

    socklen_t addrlen = sizeof(server_addr);
    if (bind(sockfd, (struct sockaddr*)&server_addr, addrlen) == -1) {
        perror("bind");
        close(sockfd);
        exit(-1);
    }

    if (listen(sockfd, 20) == -1) {
        perror("listen");
        exit(-1);
    }

    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(-1);
    }

    struct epoll_event ev, events[128];
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl");
        exit(-1);
    }
    int nfds = 0;
    int i;
    while(1) {
        nfds = epoll_wait(epfd, events, 128, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(-1);
        }
        for (i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
                if (client_fd == -1) {
                    perror("accept");
                    close(sockfd);
                    exit(-1);
                }
                printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = client_fd;
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl");
                    exit(-1);
                }
            }
            else {
                handle(events[i].data.fd, epfd, ev);
            }
        }
    }
}

void handle(int fd, int epfd, struct epoll_event ev) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t addrlen = sizeof(addr);
    getpeername(fd, (struct sockaddr*)&addr, &addrlen);

    char* pdata = (char*)malloc(BODY_SIZE);
    bzero(pdata, BODY_SIZE);

    ssize_t n = recv(fd, pdata, BODY_SIZE, 0);
    if (n == -1) {
        perror("recv");
        exit(-1);
    }

    if (n == 0) {
        printf("[%s:%d] disconnected\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev) == -1) {
            perror("epoll_ctl");
            exit(-1);
        }
        close(fd);
    }

    ConnBody body;
    bzero(&body, BODY_SIZE);
    memcpy(&body, pdata, BODY_SIZE);
    switch (body.type) {
        case 0: {
            // 发送消息
            printf("[%s:%d] %s(%d) <<<<<<: %s", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), body.name, fd, body.msg);
            if (!strcmp(body.msg, "quit\n")) {
                printf("[%s:%d] disconnected\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev) == -1) {
                    perror("epoll_ctl");
                    exit(-1);
                }
                close(fd);
            }
            break;
        }
        case 1: {
            // 登录
            printf("[%s:%d] %s(%d) : login\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), body.name, fd);
            break;
        }
        default:
            printf("unknown type\n");
    }


    free(pdata);
}




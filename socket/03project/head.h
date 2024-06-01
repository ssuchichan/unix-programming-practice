#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//报体
typedef struct ConnBody {
    int type; //0-发送消息, 1-登录
    char name[16];
    char msg[256];
} ConnBody, *PConnBody;



#define BODY_SIZE sizeof(ConnBody)

void server_epoll(char* argv[]);
void handle(int fd, int epfd,struct epoll_event ev);

#endif //HEAD_H

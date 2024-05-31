#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


void task(int fd) {
    char buf[256];
    while(1) {
        int n = recv(fd, buf, sizeof(buf), 0);
        if (n < 0) {
            perror("recv");
            exit(-1);
        }
        buf[n] = '\0';
        printf("[pid=%d] [%d]<<<<<: %s\n", getpid(), n, buf);
        if (!strcmp(buf, "quit\0")) {
            break;
        }
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: <Client> <Port>\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));
    local_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(sockfd, 20) < 0) {
        perror("listen");
        return -1;
    }
    while(1) {
        pid_t pid;
        struct sockaddr_in client_addr;
        socklen_t addrlen;
        int conn = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        if (conn < 0) {
            perror("accept");
            return -1;
        }
        printf("new connect: %s:%hu, fd = %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), conn);

        pid = fork();
        if (pid == 0) { // 子进程
            close(sockfd); //关闭父进程的fd
            task(conn);
            exit(0);
        }
        else if(pid > 0) {
            close(conn);
        }
        else {
            perror("fork");
            return -1;
        }
    }

    close(sockfd);

    return 0;
}


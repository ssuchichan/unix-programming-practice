#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_FD_NUM 1024

void remove_fd(int fd, int fds[]) {
    for (int i = 0; i < MAX_FD_NUM; ++i) {
        if (fds[i] == fd) {
            fds[i] = 0;
            break;
        }
    }
}

void add_fd(int fd, int fds[]) {
    for (int i = 0; i < MAX_FD_NUM; ++i) {
        if (fds[i] == 0) {
            fds[i] = fd;
            break;
        }
    }
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
    int client_fd[MAX_FD_NUM] = {0};
    client_fd[0] = sockfd;
    fd_set client_set;
    FD_ZERO(&client_set);
    char buf[256];
    while(1) {
        int maxfd = 0;
        for (int i = 0; i < MAX_FD_NUM; ++i) {
            if (client_fd[i] > 0) {
                FD_SET(client_fd[i], &client_set);
                if (client_fd[i] > maxfd) {
                    maxfd = client_fd[i];
                }
            }
        }

        if (select(maxfd + 1, &client_set, NULL, NULL, NULL) < 0) {
            perror("select");
            return -1;
        }

        for (int i = 0; i < MAX_FD_NUM; ++i) {
            if (FD_ISSET(client_fd[i], &client_set)) {
                if (client_fd[i] == sockfd) {
                    struct sockaddr_in addr;
                    socklen_t len = sizeof(addr);
                    int conn = accept(client_fd[i], (struct sockaddr*)&addr, &len);
                    if (conn < 0) {
                        perror("accept");
                        return -1;
                    }
                    printf("new connection: %d\n", conn);
                    add_fd(conn, client_fd);
                }
                else {
                    int n = recv(client_fd[i], buf, sizeof(buf), 0);
                    if (n < 0) {
                        perror("recv");
                        return -1;
                    }
                    buf[n] = '\0';
                    printf("fd=%d, [%d]<<<<<<: %s\n", client_fd[i], n, buf);
                    if (!strcmp(buf,"quit")) {
                        close(client_fd[i]);
                        FD_CLR(client_fd[i], &client_set);
                        remove_fd(client_fd[i], client_fd);
                    }
                }
            }
        }
    }

    close(sockfd);
    return 0;
}

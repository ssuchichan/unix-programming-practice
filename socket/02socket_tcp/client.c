#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: <Client> <IP> <Port>\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    int port =atoi(argv[2]);
    printf("server: %s:%d\n", argv[1], port);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    printf("connected\n");
    while(1) {
        char buf[256];
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        //int ret = write(sockfd, buf, strlen(buf)-1);
        int ret = send(sockfd, buf, strlen(buf)-1, 0);
        if (ret < 0) {
            perror("write");
            return -1;
        }
        if (!strcmp(buf, "quit\n")){
            break;
        }
    }

    close(sockfd);

    return 0;
}

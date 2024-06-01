#include "head.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: <Client> <IP> <Port> <Username>\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    int port =atoi(argv[2]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    printf("%s:%d connected\n", argv[1], port);
    ConnBody body;
    bzero(&body, BODY_SIZE);
    body.type = 0;
    memcpy(body.name, argv[3], strlen(argv[3]));
    while(1) {
        char data[BODY_SIZE];
        bzero(&data, BODY_SIZE);
        fgets(body.msg, sizeof(body.msg), stdin);
        memcpy(data, &body, BODY_SIZE);
        int ret = send(sockfd, data, BODY_SIZE, 0);
        if (ret < 0) {
            perror("write");
            return -1;
        }
        if (!strcmp(body.msg, "quit\n")){
            break;
        }
    }

    close(sockfd);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: <Client> <IP> <Port>\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    char buf[256];
    bzero(buf, sizeof(buf));
    strcpy(buf, "wondeful!!!");
    //fgets(buf, sizeof(buf), stdin);
    struct sockaddr_in remote_addr;
    bzero(&remote_addr, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(argv[2]));
    remote_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0) {
        perror("sendto");
        return -1;
    }

    close(sockfd);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: <Server> <Port>\n");
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local_addr;
    bzero(&local_addr, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));
    local_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("0.0.0.0");
    if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        return -1;
    }

    char buf[256];
    bzero(buf, sizeof(buf));
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));
    int ret = recvfrom(sockfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&client_addr, &addrlen);
    if (ret < 0) {
        perror("sendto");
        return -1;
    }
    buf[ret] = '\0';
    printf("ret = %d, data = %s\n", ret, buf);
    close(sockfd);

    return 0;
}

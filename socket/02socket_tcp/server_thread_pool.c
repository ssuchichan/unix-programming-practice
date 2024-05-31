#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"

sem_t sem;

void* task(void* arg) {
    struct Node* queue = (struct Node*)arg;
    char buf[256] = {0};
    while (1) {
        sem_wait(&sem);
        int fd = pop_queue(queue);
        while(1) {
            int n = recv(fd, buf, sizeof(buf), 0);
            if (n == -1) {
                perror("recv");
                exit(-1);
            }
            buf[n] = '\0';
            printf("[%ld] <<<<<<: %s\n", pthread_self(), buf);
            if (!strcmp(buf, "quit")) {
                break;
            }
        }
        close(fd);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: <Server> <Port>\n");
        return -1;
    }
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        printf("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(atoi(argv[1]));
    if (bind(listen_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return -1;
    }
    if (listen(listen_sock, 20) == -1) {
        perror("listen");
        return -1;
    }
    sem_init(&sem, 0, 0);
    struct Node* queue = (struct Node*)malloc(sizeof(struct Node));
    pthread_t tids[2] = {0};
    for (int i = 0; i < 2; ++i) {
        if (pthread_create(&tids[i], NULL, task, queue) != 0) {
            printf("pthread_create error\n");
            return -1;
        }
    }

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    while(1) {
        int client_fd = accept(listen_sock, (struct sockaddr*)&client_addr, &len);
        if (client_fd == -1) {
            perror("accept");
            return -1;
        }
        push_queue(queue, client_fd);
        sem_post(&sem);
    }

    close(listen_sock);

    return 0;
}
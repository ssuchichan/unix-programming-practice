#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func(void* arg) {
    printf("i am a new thread %ld\n", pthread_self());
    printf("arg = %d\n", *(int*)arg);
    int i;
    for (i = 0; i < 10; ++i) {
        printf("new thread: %d\n", i);
        usleep(10000);
    }
    pthread_exit(NULL);
}

int main(void) {
    pthread_t tid;
    int num = 6;
    if (0 != pthread_create(&tid, NULL, thread_func,(void*)&num)) {
        perror("pthread_create");
        exit(-1);
    }
    printf("i am a thread %ld\n", pthread_self());
    int i;
    for (i = 0; i < 10; ++i) {
        printf("main thread: %d\n", i);
        usleep(10000);
    }
    pthread_join(tid, NULL);
    return 0;
}


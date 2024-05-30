#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

int g_data = 0;
sem_t sem1,sem2;

void* func1(void* arg) {
    int i;
    for (i = 0; i < 10; ++i) {
        sem_wait(&sem2);
        g_data = g_data + 1;
        sem_post(&sem1);
        //sleep(1);
    }

    pthread_exit(NULL);
}

void* func2(void* arg) {
    int i;
    for (i = 0; i < 10; ++i) {
        sem_wait(&sem1);
        printf("g_data = %d\n", g_data);
        sem_post(&sem2);
    }

    pthread_exit(NULL);
}


int main(void) {
    pthread_t tid1, tid2;
    int ret = 0;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);
    ret = pthread_create(&tid1, NULL, func1, NULL);
    if (ret != 0) {
        printf("pthread_create error\n");
        exit(-1);
    }
    ret = pthread_create(&tid2, NULL, func2, NULL);
    if (ret != 0) {
        printf("pthread_create error\n");
        exit(-1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}

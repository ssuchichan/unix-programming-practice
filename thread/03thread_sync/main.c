#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define THREAD_COUNT 4

int g_tickets = 10000;

pthread_mutex_t g_mutext;

void* thread_func(void * arg) {
    int cnt = 0;
    while (g_tickets) {
        pthread_mutex_lock(&g_mutext); // 上锁
        if (g_tickets > 0) { // 拿到锁时g_tickets可能已经为0了，因此需再判断一次
            g_tickets = g_tickets - 1; // 临界资源
        }
        pthread_mutex_unlock(&g_mutext); // 解锁
        cnt++;
        usleep(random()%10);
    }
    printf("thread[%d] g_tickets = %d\n", (int)arg, cnt);
    pthread_exit((void*)cnt);
}

int main(void) {
    pthread_mutex_init(&g_mutext, NULL);

    pthread_t tid[THREAD_COUNT];
    int i;
    for (i = 0; i < THREAD_COUNT; ++i) {
        if (0 != pthread_create(&tid[i], NULL, thread_func, (void*)i)) {
            perror("pthread_create");
            exit(-1);
        }
    }

    int total = 0;
    void* retval;
    for (i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(tid[i], &retval);
        total += (int)retval;
    }
    printf("total = %d\n", total);

    pthread_mutex_destroy(&g_mutext);

    return 0;
}

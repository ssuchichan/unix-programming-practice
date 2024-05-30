#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// 线程可以共享数据区
int g_num = 30; // 全局变量在进程的数据段
char* g_str = NULL;

void* thread_func(void* arg) {
    printf("i am a new thread %ld, g_num = %d\n", pthread_self(), g_num);
    g_num += 1;

    // 从进程的堆中分配空间
    if (NULL == (g_str = (char*)malloc(50*sizeof(char)))) {
        pthread_exit((void*)-1);
    }
    bzero(g_str, 50);
    strcpy(g_str, "hello,world!");

    printf("arg = %d\n", *(int*)arg);
    pthread_exit(NULL);
}

int main(void) {
    pthread_t tid;
    int num = 6;
    if (0 != pthread_create(&tid, NULL, thread_func, &num)) {
        perror("pthread_create");
        exit(-1);
    }
    sleep(1);
    printf("i am a thread %ld, g_num = %d, g_str = %s\n", pthread_self(), g_num, g_str);
    pthread_join(tid, NULL);
    free(g_str);

    return 0;
}

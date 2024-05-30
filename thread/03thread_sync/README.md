# Thread Sync


互斥锁提供一个可以在同一时间，只让一个线程访问临界资源的操作接口。互斥锁是一个提供线程同步的基本锁。其中一个线程上锁后，其他线程如果想上锁，
会被阻塞，直到锁释放。如果在锁释放后，有多个线程被阻塞，那么，所有被阻塞的线程都会被设置为可执行状态。第一个执行的线程，取得锁的控制权，上锁。其他线程继续阻塞。

```
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
```
* `mutex`：互斥锁
* 返回值：成功返回0，出错返回错误编号

```
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```
* `mutex`：互斥锁
* 返回值：成功返回0，出错返回错误编号
* `trylock`：这是非阻塞调用模式，也就是说，如果互斥量没有被锁住，`trylock`将把互斥量加锁，并获取对共享资源的访问权；
如果互斥量被锁住了，`trylock`将不会等待而直接返回`EBUSY`，表示共享资源处于忙状态。


```
#include <pthread.h>
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
* `mutex`：互斥锁
* 返回值：成功返回0，出错返回错误编号

```
#include <pthread.h>
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```
* `mutex`：互斥锁
* 返回值：成功返回0，出错返回错误编号
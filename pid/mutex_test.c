#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int share_num = 0;
void increase_num();

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
  int ret;
  pthread_t thrd1, thrd2, thrd3;

  //创建线程
  ret = pthread_create(&thrd1, NULL, (void *)increase_num, NULL);
  ret = pthread_create(&thrd2, NULL, (void *)increase_num, NULL);
  ret = pthread_create(&thrd3, NULL, (void *)increase_num, NULL);

  //等待线程结束
  ret = pthread_join(thrd1, NULL);
  ret = pthread_join(thrd2, NULL);
  ret = pthread_join(thrd3, NULL);

  printf("share_num = %d\n", share_num);

  return 0;

}

void increase_num() {
  long i,tmp;
  for(i=0; i<100000; i++) {
    /*加锁*/
    if (pthread_mutex_lock(&mutex) != 0) {
      perror("pthread_mutex_lock");
      exit(-1);
    }
    tmp = share_num;
    tmp = tmp + 1;
    share_num = tmp;
    /*解锁锁*/
    if (pthread_mutex_unlock(&mutex) != 0) {
      perror("pthread_mutex_unlock");
      exit(-1);
    }
  }
}

/*
POSIX定义了一个宏PTHREAD_MUTEX_INITIALIZER来静态初始化互斥锁，方法如下： pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
 在LinuxThreads实现中，pthread_mutex_t是一个结构，而PTHREAD_MUTEX_INITIALIZER则是一个结构常量。

1.加锁：
int pthread_mutex_lock(pthread_mutex_t *mutex);
描述：
当pthread_mutex_lock()返回时，该互斥锁已被锁定。线程调用该函数让互斥锁上锁，如果该互斥锁已被另一个线程锁定和拥有，则调用该线程将阻塞，直到该互斥锁变为可用为止。
返回值：
在成功完成之后会返回零。其他任何返回值都表示出现了错误。
*/

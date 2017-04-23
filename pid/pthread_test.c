#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void print_message_function();

int main() {

  pthread_t pthread1, pthread2;
  int ret_pthread1, ret_pthread2;
  int ret_join1, ret_join2;
  char *message1 = "pthread1";
  char *message2 = "pthread2";
  void *retval;

  //创建线程
  ret_pthread1 = pthread_create(&pthread1, NULL, (void *)&print_message_function, (void *) message1);
  ret_pthread2 = pthread_create(&pthread2, NULL, (void *)&print_message_function, (void *) message2);

  //线程创建成功返回0
  if (ret_pthread1 != 0) {
    printf("线程1创建失败！\n");
    exit(-1);
  }
  if (ret_pthread2 != 0) {
    printf("线程2创建失败！\n");
    exit(-1);
  }

  //等待线程结束
  ret_join1 = pthread_join(pthread1, &retval);
  if (ret_join1 != 0) {
    printf("线程1 join失败！\n");
    exit(-1);
  }
  printf("pthread1 exit code:[%d]\n", (int)retval);

  ret_join2 = pthread_join(pthread2, &retval);
  if (ret_join2 != 0) {
    printf("线程2 join失败！\n");
    exit(-1);
  }
  printf("pthread2 exit code:[%d]\n", (int)retval);

  return 0;
}

void print_message_function( void *ptr ) {
  int i;
  for(i=0; i < 5; i++) {
    printf("%s:[%d]\n", (char *)ptr, i);
  }
  //终止线程,返回99
  pthread_exit((void *)99);
}

/*
1.创建线程：
int pthread_create(pthread_t *tidp,const pthread_attr_t *attr, (void*)(*start_rtn)(void*),void *arg);
参数说明：
  第一个参数为指向线程标识符的指针。
  第二个参数用来设置线程属性，缺省为NULL即可。
  第三个参数是线程运行函数的起始地址。
  最后一个参数是运行函数的参数。
返回值：
  若线程创建成功，则返回0.若线程创建失败，则返回出错编号.

2.函数pthread_join用来等待一个线程的结束,线程间同步的操作。
int pthread_join(pthread_t thread, void **retval);
描述：pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。
     如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。
参数说明：
  第一个参数为线程标识符，即线程ID，标识唯一线程。
  第二个参数为用户定义的指针，用来存储被等待线程的返回值。
返回值：
   0代表成功。 失败，返回的则是错误号。

3.终止线程：
void pthread_exit(void* retval);
描述：线程通过调用pthread_exit函数终止执行，就如同进程在结束时调用exit函数一样。这个函数的作用是，终止调用它的线程并返回一个指向某个对象的指针。

*/

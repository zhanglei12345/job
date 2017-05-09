#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

//函数声明
void Parent();
void Child();
void * ParentDo( char *argv );

//主函数
int main( )
{
    int pid;
    pid = fork();
    //设置第一个断点,子进程返回0，父进程返回子进程ID
    if(pid != 0)
      Parent();
    else
      Child();
    return 0;
}

//父进程
void Parent()
{
    pid_t pid = getpid();
    char cParent[] = "Parent";
    char cThread[] = "Thread";
    pthread_t pt;

    printf( "[%s]: [%d] [%s]\n", cParent, pid, "step1" );

    //创建线程
    if( pthread_create( &pt, NULL, (void *)*ParentDo, cThread ) )
    {
        printf( "[%s]: Can not create a thread.\n", cParent );
    }

    ParentDo( cParent );
    sleep(1);
}

void * ParentDo( char *argv )
{
    pid_t pid = getpid();
    //获取自己的线程号
    pthread_t tid = pthread_self();
    char tprefix[] = "thread";

    //设置第二个断点
    printf( "[%s]: [%d] [%s] [%lu] [%s]\n", argv, pid, tprefix, (unsigned long)tid, "step2" );
    printf( "[%s]: [%d] [%s] [%lu] [%s]\n", argv, pid, tprefix, (unsigned long)tid, "step3" );

    return NULL;
}

//子进程
void Child()
{
    pid_t pid = getpid();
    char prefix[] = "Child";
    printf( "[%s]: [%d] [%s]\n", prefix, pid, "step1" );
    return;
}

/*
默认设置下，在调试多进程程序时gdb只会调试主进程。gdb7以上的版本(gdb --version)支持多进程调试，只需要设置好follow-fork-mode（fork追踪模式）
以及detach-on-fork（指示GDB在fork之后是否断开某个进程的调试）即可。
这两个参数的设置命令分别是：set follow-fork-mode [parent|child]，set detach-on-fork [on|off]。两者结合起来构成了GDB的调试模式：
follow-fork-mode  detach-on-fork    说明
    parent              on          GDB默认的调试模式：只调试主进程
    child               on          只调试子进程
    parent              off         同时调试两个进程，gdb跟主进程，子进程block在fork位置
    child               off         同时调试两个进程，gdb跟子进程，主进程block在fork位置
*/

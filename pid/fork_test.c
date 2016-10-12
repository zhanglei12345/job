#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    //pid_t是个宏定义,其实质是int,被定义在#include <sys/types.h>中
    pid_t pid;
    printf("Now only one process!\n");
    printf("Calling fork...!\n");
    pid = fork();
    if(!pid)
        printf("I'm the child!\n");
    else if(pid>0)
        printf("I'm the parent,child has pid %d!\n",pid);
    else
        printf("Fork fail!\n");
    return 0;
}

//pid_t fork(void);返回值:若成功调用一次则返回两个值,子进程 返回0，父进程返回子进程ID,出错返回-1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    //pid_t是个宏定义,其实质是int,被定义在#include <sys/types.h>中
    pid_t pid, wait_pid;
    int status, i;
    printf("Now only one process!\n");
    printf("Calling fork...!\n");
    //pid_t fork(void);返回值:若成功调用一次则返回两个值,子进程 返回0，父进程返回子进程ID,出错返回-1
    pid = fork();
    if(!pid)
    {
      sleep(2);
      printf("I'm the child!\n");
      exit(2);
    }
    else if(pid > 0)
    {
      /*
      wait()会暂时停止目前进程的执行, 直到有信号来到或子进程结束. 如果在调用wait()时子进程已经结束, 则wait()会立即返回子进程结束状态值.
      子进程的结束状态值会由参数status 返回, 而子进程的进程识别码也会一快返回.
      */
      wait_pid = wait(&status);
      //WIFEXITED(status):如果子进程正常结束则为非0 值.
      if(WIFEXITED(status) != 0)
      {
        //取得子进程exit()返回的结束代码
        i = WEXITSTATUS(status);
      }
      printf("I'm the parent,child has pid [%d], [%d] ! return status:[%d],[%d]\n", pid, wait_pid, status, i);
    }
    else
      printf("Fork fail!\n");
    return 0;
}

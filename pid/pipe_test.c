#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  int pipefd[2];
  pid_t pid;
  char buf[BUFSIZ];

  //创建匿名管道
  if (pipe(pipefd) == -1) {
    perror("pipe()");
    exit(1);
  }

  //创建子进程
  pid = fork();
  if (pid == -1) {
    perror("fork()");
    exit(1);
  }

  if (pid == 0) {
    /* this is child. */
    printf("Child pid is: %d\n", getpid());
    //pipefd[0] 读，pipefd[1] 写
    if (read(pipefd[0], buf, BUFSIZ) < 0) {
      perror("write()");
      exit(1);
    }

    printf("%s\n", buf);

    memset(buf,0x00,sizeof(buf));
    snprintf(buf, BUFSIZ, "Message from child: My pid is: %d", getpid());
    if (write(pipefd[1], buf, strlen(buf)) < 0) {
        perror("write()");
        exit(1);
    }

  } else {
    /* this is parent */
    printf("Parent pid is: %d\n", getpid());

    snprintf(buf, BUFSIZ, "Message from parent: My pid is: %d", getpid());
    if (write(pipefd[1], buf, strlen(buf)) < 0) {
      perror("write()");
      exit(1);
    }

    sleep(1);

    memset(buf,0x00,sizeof(buf));
    if (read(pipefd[0], buf, BUFSIZ) < 0) {
      perror("write()");
      exit(1);
    }

    printf("%s\n", buf);

    wait(NULL);
  }

  return 0;
}

/*
1.父进程先给子进程发一个消息，子进程接收到之后打印消息，之后再给父进程发消息，父进程再打印从子进程接收到的消息。
2.int pipe(int fd[2])
函数传入值 fd[2]:管道的两个文件描述符，之后就是可以直接操作这两个文件描述符
返回值 成功 0 　失败 -1
3.一个进程在由pipe（）创建管道后，一般再fork一个子进程，然后通过管道实现父子进程间的通信
（因此也不难推出，只要两个进程中存在亲缘关系，这里的亲缘关系指的是具有共同的祖先，都可以采用管道方式来进行通信）。
4.管道两端可分别用描述字fd[0]以及fd[1]来描述，需要注意的是，管道的两端是固定了任务的。
即一端只能用于读，由描述字fd[0]表示，称其为管道读端；另一端则只能用于写，由描述字fd[1]来表示，称其为管道写端。
*/

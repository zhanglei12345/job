#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    //pid_t�Ǹ��궨��,��ʵ����int,��������#include <sys/types.h>��
    pid_t pid, wait_pid;
    int status, i;
    printf("Now only one process!\n");
    printf("Calling fork...!\n");
    //pid_t fork(void);����ֵ:���ɹ�����һ���򷵻�����ֵ,�ӽ��� ����0�������̷����ӽ���ID,������-1
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
      wait()����ʱֹͣĿǰ���̵�ִ��, ֱ�����ź��������ӽ��̽���. ����ڵ���wait()ʱ�ӽ����Ѿ�����, ��wait()�����������ӽ��̽���״ֵ̬.
      �ӽ��̵Ľ���״ֵ̬���ɲ���status ����, ���ӽ��̵Ľ���ʶ����Ҳ��һ�췵��.
      */
      wait_pid = wait(&status);
      //WIFEXITED(status):����ӽ�������������Ϊ��0 ֵ.
      if(WIFEXITED(status) != 0)
      {
        //ȡ���ӽ���exit()���صĽ�������
        i = WEXITSTATUS(status);
      }
      printf("I'm the parent,child has pid [%d], [%d] ! return status:[%d],[%d]\n", pid, wait_pid, status, i);
    }
    else
      printf("Fork fail!\n");
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    //pid_t�Ǹ��궨��,��ʵ����int,��������#include <sys/types.h>��
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

//pid_t fork(void);����ֵ:���ɹ�����һ���򷵻�����ֵ,�ӽ��� ����0�������̷����ӽ���ID,������-1

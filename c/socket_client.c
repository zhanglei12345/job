#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9001
#define MAXLINE 4096
int main(int argc, char** argv)
{
    int    sockfd, n,rec_len;
    char   recvline[4096], sendline[4096];
    char   buf[MAXLINE];
    struct sockaddr_in    servaddr;

    u_short port = PORT;

    /* 表明使用方法 */
    if(argc != 2){
      printf("Usage: ./client <ipaddress>\n");
      exit(-1);
    }

    /* 创建一个socket ,TCP传输协议 */
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
      exit(-1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    // IP地址转换函数, 将“点分十进制” －> “二进制整数”
    if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
      printf("inet_pton error for %s\n",argv[1]);
      exit(-1);
    }

    // 客户端调用connect()发出连接请求,建立与TCP服务器的连接
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
      printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
      exit(-1);
    }

    while (1) {

      printf("\nsend msg to server: \n");
      memset(sendline,0,sizeof(sendline));
      fgets(sendline, 4096, stdin);
      printf("sendline:[%s]\n", sendline);

      if( send(sockfd, sendline, strlen(sendline), 0) < 0)
      {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        exit(-1);
      }
      if(strcmp(sendline, "quit\n") == 0) {
        printf("close client \n");
        close(sockfd);
        break;
      }
      if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {
        perror("recv error");
        exit(1);
      }
      buf[rec_len]  = '\0';
      printf("Received : [%s] ",buf);
    }

    return 0;
}

/*
  将“点分十进制” －> “二进制整数”:
  int inet_pton(int af, const char *src, void *dst);
  af = AF_INET,src为指向字符型的地址，即ASCII的地址的首地址（ddd.ddd.ddd.ddd格式的），函数将该地址转换为in_addr的结构体，并复制在*dst中。
  如果函数出错将返回一个负值，并将errno设置为EAFNOSUPPORT，如果参数af指定的地址族和src格式不对，函数将返回0.

  将“二进制整数” －> “点分十进制”:
  const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
  这个函数转换网络二进制结构到ASCII类型的地址，参数的作用和inet_pton相同，只是多了一个参数socklen_t cnt,他是所指向缓存区dst的大小，避免溢出,
  如果缓存区太小无法存储地址的值，则返回一个空指针，并将errno置为ENOSPC。
*/

/*
  TCP服务器端依次调用socket()、bind()、listen()之后，就会监听指定的socket地址了。
  TCP客户端依次调用socket()、connect()之后就向TCP服务器发送了一个连接请求。
  TCP服务器监听到这个请求之后，就会调用accept()函数取接收请求，这样连接就建立好了。
  之后就可以开始网络I/O操作了，即类同于普通文件的读写I/O操作.
*/

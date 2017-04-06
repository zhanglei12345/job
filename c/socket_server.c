#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 1024

#define SERVER_STRING "Server: hoohackhttpd/0.1.0\r\n"

int main()
{
	/* 定义server和client的套接字描述符 */
	int server_fd = -1;
	int client_fd = -1;

	u_short port = PORT;
	/* 套接字地址结构 */
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	char buf[BUFF_SIZE];   //1024
	char recv_buf[BUFF_SIZE];
	char hello_str[] = "Hello world!";

	int hello_len = 0;

	/* 创建一个socket */
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1){
		perror("socket");	//perror(s) 用来将输入的信息和上一个函数发生错误的原因一起输出到标准设备(stderr)。
		exit(-1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	/* 设置端口，IP，和TCP/IP协议族 */
	server_addr.sin_family = AF_INET;	//协议族
	server_addr.sin_port = htons(PORT);  //端口：9001     htons()是将整型变量从主机字节顺序转变成网络字节顺序
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP地址  htonl()将主机数转换成无符号长整型的网络字节顺序
														//INADDR_ANY就是指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或“所有地址”、“任意地址”
	/* 绑定套接字到端口 */
	if(bind(server_fd, (struct sockaddr *)&server_addr,sizeof(server_addr)) < 0){
		perror("bind");
		exit(-1);
	}

	/* listen函数会将套接字从CLOSED状态转换到LISTEN状态，第二个参数规定内核应该为相应套接字排队的最大连接个数。*/
	if(listen(server_fd, QUEUE_MAX_COUNT) < 0){
		perror("listen");
		exit(-1);
	}

	printf("http server running on port %d\n", port);

	while(1){
		/* 调用了accept函数，阻塞了程序，直到接收到客户端的请求 */
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(client_fd < 0){
			perror("accept");
			exit(-1);
		}
		printf("accept a client\n");

		printf("client socket fd: %d\n", client_fd);
		/* 调用recv函数接收客户端发来的请求信息 */
		/* recv()返回读入的字节数。如果连接已中止，返回0。如果发生错误，返回-1 */
		hello_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);

		printf("receive %d\n", hello_len);

		/* 发送响应给客户端 */
		sprintf(buf, "HTTP/1.0 200 OK\r\n");
		send(client_fd, buf, strlen(buf), 0);
		strcpy(buf, SERVER_STRING);
		send(client_fd, buf, strlen(buf), 0);
		sprintf(buf, "Content-Type: text/html\r\n");
		send(client_fd, buf, strlen(buf), 0);
		strcpy(buf, "\r\n");
		send(client_fd, buf, strlen(buf), 0);
		sprintf(buf, "Hello World\r\n");
		send(client_fd, buf, strlen(buf), 0);

		/* 关闭客户端套接字 */
		close(client_fd);
	}

	close(server_fd);
	return 0;
}


/*
 * int socket(int family, int type, int protocol);
   family：协议族。AF_INET：IPV4协议；AF_INET6：IPv6协议；AF_LOCAL：Unix域协议；AF_ROUTE：路由套接字；AF_KEY：密钥套接字
   type：套接字类型。SOCK_STREAM ： 字节流套接字；SOCK_DGRAM：数据包套接字；SOCK_SEGPACKET：有序分组套接字；SOCK_RAW：原始套接字
   protocol：某个协议类型常量。TCP：0，UDP ：1， SCTP ：
   返回值：如果调用成功就返回新创建的套接字的描述符(整型)，如果失败就返回INVALID_SOCKET（Linux下失败返回-1）,根据套接字描述符就可以找到其对应的套接字数据结构
 *
 * ipv4 套接字地址结构:
   struct sockaddr_in {
        uint8_t            sin_len;         结构体的长度 
     	sa_family_t        sin_family;      IP协议族，IPV4是AF_INET 
	    in_port_t          sin_port;        一个16比特的TCP/UDP端口地址 
	    struct in_addr     sin_addr;        32比特的IPV4地址，网络字节序 
	    char               sin_zero[8];     未使用字段 
	}; 注：sockaddr_in是**Internet socket address structure**的缩写。
 * IP地址结构:
	struct in_addr {
	     in_addr_t      s_addr;
 	};
 * int bind(int, struct sockaddr *, socklen_t);
   返回值:如果函数执行成功，返回值为0，否则为SOCKET_ERROR。
*/

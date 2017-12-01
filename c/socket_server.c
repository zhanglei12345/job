#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PORT 9001
#define QUEUE_MAX_COUNT 5
#define BUFF_SIZE 4096

#define SERVER_STRING "Server: \r\n"

void exit_handler(int sigal)
{
	//接受SIGCHLD信号
	int status;
	int pid;
	/*
	-1 等待任何子进程,相当于 wait()。
	WNOHANG 若pid指定的子进程没有结束，则waitpid()函数返回0，不予以等待。若结束，则返回该子进程的ID。
	*/
	pid = waitpid(-1,&status,WNOHANG);
	//WIFEXITED(status):如果子进程正常结束则为非0值.
	if(WIFEXITED(status))
	{
		//取得子进程exit()返回的结束码
		printf("The child pid:[%d] exit with code:[%d]\n",pid,WEXITSTATUS(status));
	}
}

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

	char buf[BUFF_SIZE];   //4096
	char addr_p[32];  //IP地址的点分十进制字符串表示形式
	char recv_buf[BUFF_SIZE];

	int hello_len = 0;
	long pid;

	//回收僵尸进程
	//子进程退出的时候，会发送SIGCHLD信号
	signal(SIGCHLD, exit_handler);

	/* 创建一个socket, 监听套接字, TCP传输协议 */
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0){
		perror("socket");	//perror(s) 用来将输入的信息和上一个函数发生错误的原因一起输出到标准设备(stderr)。
		exit(-1);
	}
	memset(&server_addr, 0, sizeof(server_addr));
	/* 设置端口，IP，和TCP/IP协议族 */
	/*
		如果指定端口号为0，那么内核就在bind被调用时选择一个临时端口。
		然而如果指定IP地址为通配地址，那么内核将等到套接字已连接（TCP）或已在套接字上发出数据报（UDP）时才选择一个本地IP地址。
		对于IPv4来说，通配地址由常值INADDR_ANY来指定，其值一般为0，它告知内核去选择IP地址。
	*/
	server_addr.sin_family = AF_INET;	//协议族
	server_addr.sin_port = htons(PORT);  //端口：9001     htons()是将主机字节序转变成网络字节序.  h代表host,n代表network,s代表short,l代表long
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP地址  htonl()将主机字节序转换成网络字节序

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

	if (inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, addr_p, (socklen_t )sizeof(addr_p)) == NULL) {
		perror("inet_ntop");
		exit(-1);
	}
	printf("http server running on address %s, port %d\n", addr_p, port);

	while(1){
		/* 调用了accept函数，阻塞了程序，直到接收到客户端的请求, 返回一个新可用的连接套接字 */
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(client_fd < 0){
			perror("accept");
			continue;
		}
		printf("accept a client\n");

		printf("client socket fd: %d\n", client_fd);
		/*
		 用fork来同时服务多个客户端，父进程专门负责监听端口，每次accept一个新的客户端连接就fork出一个子进程专门服务这个客户端。
		 父子进程共享打开的文件描述符，因为在子进程已经用不到监听描述符，故将其关闭，而连接描述符对父进程也没价值，将其关闭.
		 if(fork() > 0)
		 {
			 close(client_fd);
			 continue;
		 }
		 close(server_fd);
		 ...(子进程处理)
		*/
		if((pid = fork()) > 0)
		{
			close(client_fd);
			continue;
		}
		//子进程
		if(pid == 0) {
			close(server_fd);
			//一直接收客户端socket的send操作
			while (1) {
				memset(recv_buf, 0, sizeof(recv_buf));
				/* 调用recv函数接收客户端发来的请求信息 */
				/* recv()返回读入的字节数。如果连接已中止，返回0。如果发生错误，返回-1 */
				hello_len = recv(client_fd, recv_buf, BUFF_SIZE, 0);

				//去除行尾的'\n'
				recv_buf[hello_len-1] = '\0';

				printf("receive [%s] len:%d\n", recv_buf, hello_len);

				if (strcmp(recv_buf,"quit") == 0) {
					printf("break\n");
					break;
				}
				/* 发送响应给客户端 */
				/*
				sprintf(buf, "HTTP/1.0 200 OK\r\n");
				send(client_fd, buf, strlen(buf), 0);
				strcpy(buf, SERVER_STRING);
				send(client_fd, buf, strlen(buf), 0);
				sprintf(buf, "Content-Type: text/html\r\n");
				send(client_fd, buf, strlen(buf), 0);
				strcpy(buf, "\r\n");
				send(client_fd, buf, strlen(buf), 0);
				*/
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "Hello [%d]", hello_len);
				if(send(client_fd, buf, strlen(buf), 0) == -1) {
					perror("send");
					close(client_fd);
					exit(-1);
				}
			}
			/* 关闭客户端套接字 */
			close(client_fd);
			exit(0);
		}
	}

	return 0;
}


/*
 * int socket(int family, int type, int protocol);
   family：协议族。AF_INET：IPV4协议；AF_INET6：IPv6协议；AF_LOCAL：Unix域协议；AF_ROUTE：路由套接字；AF_KEY：密钥套接字
   type：套接字类型。SOCK_STREAM ： 字节流套接字；SOCK_DGRAM：数据报套接字；SOCK_SEGPACKET：有序分组套接字；SOCK_RAW：原始套接字
   protocol：某个协议类型常量值。设置为0表示选择所给定family和type组合的系统默认值。
	 					AF_INET或AF_INET6的protocol常值：IPPROTO_TCP：TCP传输协议，IPPROTO_UDP：UDP传输协议，IPPROTO_SCTP：SCTP传输协议
   返回值：如果调用成功就返回新创建的套接字描述符(一个小的非负整数值)，简称sockfd，如果失败就返回INVALID_SOCKET（Linux下失败返回-1）,根据套接字描述符就可以找到其对应的套接字数据结构
 *
 * ipv4 套接字地址结构(网际套接字地址结构):
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

 * int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 参数sockfd:
  套接字描述符，该套接口在listen()后监听连接。客户不知道套接字这些细节，它只知道一个地址和一个端口号。
 参数addr:
	这是一个结果参数，它用来接受一个返回值，这返回值指定客户端的地址，当然这个地址是通过某个地址结构来描述的，用户应该知道这一个什么样的地址结构。
	如果对客户的地址不感兴趣，那么可以把这个值设置为NULL。
 参数addrlen:
	如同大家所认为的，它也是结果的参数，用来接受上述addr的结构的大小的，它指明addr结构所占有的字节个数。同样的，它也可以被设置为NULL。
*/

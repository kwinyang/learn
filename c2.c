#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <fcntl.h>
#define MAXLINE 10
#define PORT 8000

int main(void){
	//定义客户端套接字
	int sockfd = -1;
	//定义想连接的服务器的套接字地址
	struct sockaddr_in servaddr;

	//发送和接收数据的缓冲区
	char sendbuf[MAXLINE], recbuf[MAXLINE];

	//初始化服务器套接字地址
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;//IPv4
	servaddr.sin_port = htons(PORT);//想连接的服务器的端口
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器的IP地址

	//创建套接字
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("create socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}

	//向服务器发送连接请求
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		//连接失败
		printf("connect socket error: %s(error: %d)\n", strerror(errno), errno);
		exit(0);
	}
         int x;  
    x=fcntl(sockfd,F_GETFL,0);  
    fcntl(sockfd,F_SETFL,x | O_NONBLOCK);  
	while(1){
		//向服务器发送信息
		printf("向服务器发送信息：");
		fgets(sendbuf, sizeof(sendbuf), stdin);
		write(sockfd, sendbuf, sizeof(sendbuf));

		//从服务器接收信息
		ssize_t len = read(sockfd, recbuf, sizeof(recbuf));
		if(len < 0){
			if(errno == EINTR){
				continue;
			}
			exit(0);
		}

		printf("服务器回应：%s\n", recbuf);
	}

	//关闭套接字
	close(sockfd);

}

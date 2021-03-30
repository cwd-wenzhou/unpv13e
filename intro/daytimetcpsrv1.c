#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			len;
	struct sockaddr_in	servaddr, cliaddr;
	char				buff[MAXLINE];
	time_t				ticks;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);//tcp连接，创建套接字

	//创建本地协议tcp
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	//绑定本地协议和套接字
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	//把套接字（默认为主动套接字）转化为被动套接字，让内核接受指向该套接字的连接请求
	//LISTENQ==1024 内核为该套接字排队的最大连接个数
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);//返回的是已完成的连接队列的队头
		printf("connection from %s, port %d\n",
			   Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));//按照format的格式格式化为字符串，然后再将其拷贝至str中。

		char *p1 = "Hello world\n";
        Write(connfd, p1, strlen(p1));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}

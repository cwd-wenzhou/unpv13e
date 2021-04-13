//链接dgcliaddr.cpp.增加忽略其他端口信息的功能
#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: udpcli <IPaddress>");

	bzero(&servaddr, sizeof(servaddr));
#ifdef	HAVE_SOCKADDR_SA_LEN
	servaddr.sin_len = sizeof(servaddr);
#endif
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7);//改为标准回射服务器的端口
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

	exit(0);
}

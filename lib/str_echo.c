#include	"unp.h"

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];
    //printf("run lb echo");
again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0){
		//strcpy(buf,"cwd");
		Writen(sockfd, buf, n);
	}
		

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}

#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	//printf("run lb");
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		Writen(sockfd, sendline, strlen(sendline));

		if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");
		//Fputs("hello ", stdout);
		strcat(recvline,"--send by cwd");
		Fputs(recvline, stdout);
	}
}

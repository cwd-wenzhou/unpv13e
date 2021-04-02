#include	"unp.h"

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0){
		Writen(sockfd, buf, n);
	}
		

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");

	// for ( ; ; ) {
	// 	if ( (n = Readline(sockfd, buf, MAXLINE)) == 0)
	// 		return;		/* connection closed by other end */

	// 	strcat(buf,"send in echo");
		
	// 	//snprintf(buf, sizeof(buf), "input error\n");

	// 	n = strlen(buf);
	// 	printf("here is buf:%s the length of buf=%d",buf,n);
	// 	Writen(sockfd, buf, n);
	// }
}

#include	"unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	stdineof = 0;
	FD_ZERO(&rset);
	for ( ; ; ) {
		//若stdineof==0，标准输入可读
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);

		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
				if (stdineof == 1)
					return;		/*已经输入过EOF，是 normal termination */
				else
					err_quit("str_cli: server terminated prematurely");
			}

			Write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
		//调用read时已到达文件末尾，返回值为0，进入if里面
			if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR);	/* send FIN   SHUT_WR：关闭连接写的这一半*/
				FD_CLR(fileno(fp), &rset);
				continue;
				//从此以后只接受数据，不发送数据
			}

			Writen(sockfd, buf, n);
		}
	}
}

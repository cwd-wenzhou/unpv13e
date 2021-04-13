#include	"unp.h"

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	//从标准输入读进来
	while (Fgets(sendline, MAXLINE, fp) != NULL) {
		//将该文本发送给服务器
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		//接受服务器的回射
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);//两个NULL表示并不关心应答数据包是谁发送的

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);//把回射的文本显示到标准输出
	}
}

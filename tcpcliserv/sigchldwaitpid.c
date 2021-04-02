#include	"unp.h"

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;
   //WNOHANG表示在尚有未终止的进程的时候不要阻塞
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}

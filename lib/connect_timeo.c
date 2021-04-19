/* include connect_timeo */
//可能干扰进程中现有的alarm调用，仅仅适合在未线程化和单线程的程序中使用
#include	"unp.h"

static void	connect_alarm(int);

int
connect_timeo(int sockfd, const SA *saptr, socklen_t salen, int nsec)
{
	Sigfunc	*sigfunc;
	int		n;

	sigfunc = Signal(SIGALRM, connect_alarm);//建立一个信号处理函数。储存现有的信号处理函数
	if (alarm(nsec) != 0)//alarm设置报警时钟，若已设置过，返回值为当前剩余秒数，否则为0。
	//有返回值时输出一下报警，因为重置时间了。
		err_msg("connect_timeo: alarm was already set");

	if ( (n = connect(sockfd, saptr, salen)) < 0) {
		close(sockfd);
		if (errno == EINTR)
			errno = ETIMEDOUT;//若被中断打断，就是超时，把EINTR改为ETIMEDOUT。
	}
	alarm(0);					/* turn off the alarm */
	Signal(SIGALRM, sigfunc);	/* restore previous signal handler */

	return(n);
}

static void
connect_alarm(int signo)
{
	return;		/* just interrupt the connect() */
}
/* end connect_timeo */

void
Connect_timeo(int fd, const SA *sa, socklen_t salen, int sec)
{
	if (connect_timeo(fd, sa, salen, sec) < 0)
		err_sys("connect_timeo error");
}

#include	"web.h"

void
start_connect(struct file *fptr)
{
	int				fd, flags, n;
	struct addrinfo	*ai;
	//char ip[14];	
	ai = Host_serv(fptr->f_host, SERV, 0, SOCK_STREAM);
	//Host_serv把“www.gitbuh.com”这样的主机名转化成addrinfo类型的ai

	printf(" ai->ai_canonname=      %s\n", ai->ai_canonname);

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)ai->ai_addr;
	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
	printf("The IP address is: %s\n", ipAddress);


	fd = Socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	fptr->f_fd = fd;
	printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

		/* 4Set socket nonblocking */
	flags = Fcntl(fd, F_GETFL, 0);
	Fcntl(fd, F_SETFL, flags | O_NONBLOCK);

		/* 4Initiate nonblocking connect to the server. */
	if ( (n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0) {
		if (errno != EINPROGRESS)
			err_sys("nonblocking connect error");
		fptr->f_flags = F_CONNECTING;
		FD_SET(fd, &rset);			/* select for reading and writing */
		FD_SET(fd, &wset);
		if (fd > maxfd)
			maxfd = fd;

	} else if (n >= 0)				/* connect is already done */
	//connect成功返回，连接已经建立，调用write_get_cmd发GET命令给服务器
		write_get_cmd(fptr);	/* write() the GET command */
}

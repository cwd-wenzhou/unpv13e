/* include fig01 */
#include	"unp.h"
#include	<limits.h>		/* for OPEN_MAX */
#include  <sys/epoll.h>
#define INET_ADDRSTRLEN 16
#define OPEN_MAX  FOPEN_MAX
#define NOTDEF
int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	char				buf[MAXLINE];
    char                str[INET_ADDRSTRLEN];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
    //创建套接字
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    //绑定套接字
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    //开启监听
	Listen(listenfd, LISTENQ);

     //step1: epoll开始工作之前 先把文件描述符纳入epoll监管
    struct epoll_event  event;
    event.data.fd=listenfd;
    event.events=EPOLLIN| EPOLLET;    
    int epoll_fd=epoll_create(10);
    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listenfd,&event);

    
    struct epoll_event events[10];
    printf("run here\n");

/* include fig02 */
	for ( ; ; ) {
        //step2: epoll开始工作 阻塞的等待文件描述符就绪
        int ret=epoll_wait(epoll_fd,events,10,-1);

        //管他马的，先打印ret的值看看
       // printf("ret=%d\n",ret);

        //step3:epoll完成工作  看自己感兴趣的套接字是否就绪
        if(ret<0){
            //epoll返回-1表示出错
            printf("epoll error\n");
        }
        else if(ret==0){
            //epoll返回0表示超时
            printf("epoll time out\n");
        }
        else{
            //返回的ret>0时 表示就绪的描述符个数
            for(int i=0;i<ret;i++){
                if ( (events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || 
				(!(events[i].events & EPOLLIN))) {
					printf("epoll error\n");
					close(events[i].data.fd);
					continue;
			    }
                else if(events[i].data.fd==listenfd) //有新的连接
                {
                    connfd = Accept(listenfd, (SA *) &cliaddr, &clilen); //accept这个连接
                    //printf("new client\n");
                    printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));//输出新连接的地址信息和端口号

                    event.data.fd=connfd;
                    event.events=EPOLLIN|EPOLLET;
                    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,connfd,&event); //将新的fd添加到epoll的监听队列中
                }
    
                else if( events[i].events&EPOLLIN ) //接收到数据，读socket
                {
                    connfd=events[i].data.fd;
                    int bytes = read(connfd,buf,MAXLINE);
                    // 客户端关闭连接
                    if (bytes == 0){
                        ret =epoll_ctl(epoll_fd,EPOLL_CTL_DEL,connfd,NULL);
                        close(connfd);
                        printf("client[%d] closed\n", i);
                    }
                    else
                        write(connfd,buf,bytes);
                    
                    //event.data.fd=connfd;     //md为自定义类型，添加数据
                    //event.events=EPOLLOUT|EPOLLET;
                    //epoll_ctl(epoll_fd,EPOLL_CTL_MOD,listenfd,&event);//修改标识符，等待下一个循环时发送数据，异步处理的精髓
                }

            }
	    }
        //printf("endif\n");
    }
}
/* end fig02 */

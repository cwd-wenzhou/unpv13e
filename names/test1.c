#include	"unp.h"

void	pr_ipv4(char **);

int
main(int argc, char **argv)
{
	int				listenfd, n;
	const int		on = 1;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	n = getaddrinfo(NULL, argv[1], &hints, &res);
	printf("res->ai_canonname=%s",res->ai_canonname);

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);
	printf("The IP port is: %d\n", ntohs(ipv4->sin_port));
	printf("The IP address is: %s\n", ipAddress);

	//printf("res->ai_canonname=%s",res->ai_addrlen);
	ressave = res;
	// char			*ptr, **pptr, **listptr, buf[INET6_ADDRSTRLEN];
	// char			*list[100];
	// int				i, addrtype, addrlen;
	// struct hostent	*hptr;

	// while (--argc > 0) {
	// 	ptr = *++argv;
	// 	if ( (hptr = gethostbyname(ptr)) == NULL) {
	// 		err_msg("gethostbyname error for host: %s: %s",
	// 				ptr, hstrerror(h_errno));
	// 		continue;
	// 	}
	// 	printf("official host name: %s\n", hptr->h_name);

	// 	for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
	// 		printf("	alias: %s\n", *pptr);
	// 	addrtype = hptr->h_addrtype;
	// 	addrlen = hptr->h_length;

	// 		/* copy array of pointers, so we can call gethostbyaddr() */
	// 	for (i = 0, listptr = hptr->h_addr_list; *listptr != NULL; listptr++) {
	// 		list[i++] = *listptr;
	// 	}
	// 	list[i] = NULL;

	// 	for (listptr = list; *listptr != NULL; listptr++) {
	// 		printf("\taddress: %s\n",
	// 			   Inet_ntop(addrtype, *listptr, buf, sizeof(buf)));

	// 		if ( (hptr = gethostbyaddr(*listptr, addrlen, addrtype)) == NULL)
	// 			printf("\t\t(gethostbyaddr failed)\n");
	// 		else if (hptr->h_name != NULL)
	// 			printf("\t\tname = %s\n", hptr->h_name);
	// 		else
	// 			printf("\t\t(no hostname returned by gethostbyaddr)\n");

	// 		printf("\t\tofficial host name: %s\n", hptr->h_name);

	// 		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
	// 			printf("\t\talias: %s\n", *pptr);
	// 	}
	// }
}

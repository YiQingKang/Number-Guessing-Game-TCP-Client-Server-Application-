#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
    int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE], username[30];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9999);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; )
    {
		connfd = Accept(listenfd, (SA *) NULL, NULL);
        Read(connfd, username, 30);
        printf("Connection from %s \n", username);
        
        for ( ; ; )
        {
            bzero(buff, MAXLINE);
            Read(connfd, buff, MAXLINE);
            
            if(strncmp(buff, "Server, what time is it?", 24) == 0)
            {
                bzero(buff, MAXLINE);
                ticks = time(NULL);
                snprintf(buff, MAXLINE, "%.24s\r\n", ctime(&ticks));
                printf("Current time is: %.24s\r\n", ctime(&ticks));
                Write(connfd, buff, MAXLINE);
            }
            else if(strncmp(buff, "Thank you server.", 17) == 0)
            {
                printf("You're welcome. Bye %s.\n", username);
                Write(connfd, "Connection terminated.", 22);
                break;
            }
            else
            {
                Write(connfd, "Invalid command.", 16);
            }
        }
        
		Close(connfd);
	}
}

#include "csapp.h"

#define PORT 2121

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(0);
    }

    host = argv[1];

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, PORT);
    
    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("client connected to server OS\n"); 
    
    Rio_readinitb(&rio, clientfd);

    Fputs("ftp >",stdout);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        //printf("ftp > ");
        if(strcmp(buf,"exit\n") == 0) {
            printf("Exiting ftp\n");
            exit(0);
        }

        Rio_writen(clientfd, buf, strlen(buf));
        
        if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
            Fputs("Server response : ",stdout);
            Fputs(buf, stdout);
        } else { /* the server has prematurely closed the connection */
            printf("The server closed the connection\n");
            break;
        }
        Fputs("ftp >",stdout);
    }

    Close(clientfd);
    exit(0);
}

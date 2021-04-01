#include "csapp.h"
#include "treatment.h"

void command_treatment(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %u bytes\n", (unsigned int)n);
        printf("Client command : %s",buf);
        Rio_writen(connfd, buf, n);
    }
}
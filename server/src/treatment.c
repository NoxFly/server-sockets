#include "csapp.h"
#include "treatment.h"

#include <unistd.h>

void command_treatment(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    char msg[MAXLINE];
    char header[MAXLINE];
    char file_location[MAXLINE + strlen(STORAGE_LOCATION)];
    rio_t rio;
    int req_file;

    Rio_readinitb(&rio, connfd);
    /*
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %u bytes\n", (unsigned int)n);
        printf("Client command : %s",buf);
        Rio_writen(connfd, buf, n);
    }
    */
    n = Rio_readlineb(&rio, buf,MAXLINE);
    strcat(file_location,STORAGE_LOCATION);
    strcat(file_location,buf);
    file_location[strlen(file_location) -1] = '\0';
    printf("Client require file %s\n",file_location);
    if((req_file = open(file_location,0600,O_RDONLY)) < 0){
        printf("Failed to open file %s\n",file_location);
        strcat(header,"error\n");
        Rio_writen(connfd,header,strlen(header));
        strcat(msg,"Failed to open this file\n");
        Rio_writen(connfd,msg,strlen(msg));
    }
    else{
        printf("File opened\n");
        strcat(header,"success\n");
        printf("Sending success\n");
        Rio_writen(connfd,header,strlen(header));
        strcat(msg,"file opened\n");
        printf("Sending file\n");
        Rio_writen(connfd,msg,strlen(msg));
    }
}
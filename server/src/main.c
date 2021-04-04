#include "csapp.h"
#include "treatment.h"
#include "handler.h"

#include "server.h"

/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    int pools[POOL];
    pid_t pid = getpid();
    
    if(argc != 1) {
        printf("Too much arguments...\n");
        exit(1);
    }
    
    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(PORT);

    for(int i = 0; i< POOL; i++){
        if(pid != 0){
            pid = Fork();
        }
    }

    if(pid == 0){
        Signal(SIGINT,exit_handler_child);
    }
    else{
        Signal(SIGINT,exit_handler_main);
    }

    while (1) {
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        /* determine the name of the client */
        Getnameinfo((SA *) &clientaddr, clientlen,
                    client_hostname, MAX_NAME_LEN, 0, 0, 0);
        
        /* determine the textual representation of the client's IP address */
        Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                  INET_ADDRSTRLEN);
        
        printf("server connected to %s (%s)\n", client_hostname,
               client_ip_string);

        command_treatment(connfd);
        //Close(connfd);
    }

    exit(0);
}
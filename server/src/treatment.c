#include "csapp.h"
#include "treatment.h"

#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long file_size(char* req_file, struct stat dest){
    stat(req_file,&dest);
}

char * file_content(int req_file, long long size, char* content){
    rio_t file_rio;
    Rio_readinitb(&file_rio,req_file);
    Rio_readnb(&file_rio,content,size);
    Close(req_file);
}

int len(char* string){
    int nb = 0;
    while (*string)
    {
        nb++;
        *string++;
    }
    return nb;
}

void command_treatment(int connfd)
{
    size_t n,m;
    char file_location[MAXLINE + strlen(STORAGE_LOCATION)];
    char content[MAXLINE];
    rio_t rio, file_rio;
    long lg;
    int req_file;
    struct stat buffer;
    REQ_MSG req = malloc(sizeof(REQ_MSG));
    REP_MSG rep = malloc(sizeof(REP_MSG));

    printf("___________________________\n");

    Rio_readinitb(&rio, connfd);
    n = Rio_readnb(&rio, req,sizeof(REQ_MSG));
    printf("Requete reçue : \n\tnum commande : %d\n\targument : %d\n",req->cmd,req->arg1);
    printf("\n");
    char file_name[req->arg1];
    m = Rio_readnb(&rio,file_name,req->arg1);
    file_name[req->arg1] = 0;
    printf("File name : %s\n",file_name);
    strcat(file_location,STORAGE_LOCATION);
    strcat(file_location,file_name);
    printf("Client require file %s\n",file_location);
    
    req_file = open(file_location,S_IWUSR,O_RDONLY);
    if(req_file == -1){
        printf("File not found\n");
        rep->size = -1;
        rep->state = 0;
        strcat(content,"Failed to open the file");
        Rio_writen(connfd,rep,sizeof(REP_MSG));
        Rio_writen(connfd,content,len(content));
    }
    else{
        printf("File found\n");
        file_size(file_location,buffer);
        file_content(req_file,(long long)buffer.st_size,content);
        rep->size = (long long)buffer.st_size;
        rep->state = 1;
        Rio_writen(connfd,rep,sizeof(REP_MSG));
        Rio_writen(connfd,content,lg);
        close(req_file);
    }
}
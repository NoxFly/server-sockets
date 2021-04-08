#include "csapp.h"
#include "treatment.h"

#include <unistd.h>
#include <math.h>

long file_size(FILE* req_file){
    fseek(req_file,0,SEEK_END);
    long numbyte = ftell(req_file);
    fseek(req_file,0,SEEK_SET);
    return numbyte;
}

char * file_content(FILE* req_file, long size){
    char *content = malloc(size);
    fread(content,1,size,req_file);
    fclose(req_file);
    //content[size -1] = 10;
    return content;
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
    char* content;
    rio_t rio;
    long lg;
    FILE* req_file;
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
    
    req_file = fopen(file_location,"rb");
    if(req_file == NULL){
        printf("File not found\n");
        rep->size = -1;
        rep->state = 0;
        content = "Failed to open the file";
        Rio_writen(connfd,rep,sizeof(REP_MSG));
        Rio_writen(connfd,content,len(content));
    }
    else{
        printf("File found\n");
        lg = file_size(req_file);
        content = file_content(req_file,lg);
        rep->size = lg;
        rep->state = 1;
        Rio_writen(connfd,rep,sizeof(REP_MSG));
        Rio_writen(connfd,content,lg);
    }
}
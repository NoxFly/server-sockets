#include "csapp.h"
#include "treatment.h"

#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int file_size(char *req_file)
{
    FILE *f = fopen(req_file, "r");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fclose(f);
    return size;
}

char *file_content(rio_t file_rio, int size, char *content)
{
    Rio_readnb(&file_rio, content, size);
}

int len(char *string)
{
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
    size_t n, m;
    char file_location[MAXLINE + strlen(STORAGE_LOCATION)];
    char content[MAXLINE];
    rio_t rio, file_rio;
    //size_t lg;
    int size;
    int req_file;
    struct stat *buffer;
    REQ_MSG req = (REQ_MSG)malloc(sizeof(REQ_MSG));
    REP_MSG rep = (REP_MSG)malloc(sizeof(REP_MSG));

#ifdef DEBUG
    printf("___________________________\n");
#endif

    Rio_readinitb(&rio, connfd);
    n = Rio_readnb(&rio, req, sizeof(REQ_MSG));

#ifdef DEBUG
    printf("Requete reçue : \n\tnum commande : %d\n\targument : %d\n", req->cmd, req->arg1);
    printf("\n");
#endif

    char file_name[req->arg1];
    m = Rio_readnb(&rio, file_name, req->arg1);
    file_name[req->arg1] = 0;

    strcat(file_location, STORAGE_LOCATION);
    strcat(file_location, file_name);

#ifdef DEBUG
    printf("File name : %s\n", file_name);
    printf("Client requires file %s\n", file_location);
#endif

    req_file = open(file_location, O_RDONLY);

    if (req_file == -1)
    {
#ifdef DEBUG
        printf("File not found\n");
#endif

        rep->size = -1;
        rep->state = 0;

        strcat(content, "Failed to open the file");
        Rio_writen(connfd, rep, sizeof(REP_MSG));
        Rio_writen(connfd, content, len(content));
    }
    else
    {
#ifdef DEBUG
        printf("File found\n");
#endif
        stat(file_location, buffer);
        size = file_size(file_location);
        int nb_rep = (size / MAXBUF) + 1;

        printf("Size with stat : %lld\n", buffer->st_size);

        //file_content(req_file, size, content);

        //rep->size = lg;
        rep->size = size;
        rep->state = 1;

#ifdef DEBUG
        printf("Structure envoyée : \n\tstatus : %d\n\tsize : %d\n", rep->state, rep->size);
#endif

        Rio_writen(connfd, rep, sizeof(REP_MSG));
        Rio_readinitb(&file_rio, req_file);
        for (int i = 0; i < (size / MAXLINE) + 1; i++)
        {
            if (i == (size / MAXLINE))
            {
                if (MAXLINE / size != 0)
                {
                    file_content(file_rio, size, content);
                    Rio_writen(connfd, content, size);
                }
            }
            else
            {
                file_content(file_rio, MAXLINE, content);
                Rio_writen(connfd, content, MAXLINE);
            }
        }

        Close(req_file);
    }
}
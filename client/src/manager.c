#include "manager.h"
#include <string.h>

#include <time.h>

char **str_split(char *a_str, const char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char *) * count);

    if (result)
    {
        size_t idx = 0;
        char *token = strtok(a_str, delim);

        while (token)
        {
            //assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
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

void save_content(int fd, char *content, int size)
{
    Rio_writen(fd, content, size);
}

char **create_request(char *buf, REQ_MSG req)
{
    char **args = str_split(buf, ' ');
    if (strcmp(args[0], "get") == 0)
    {
        req->cmd = 1;
        req->arg1 = len(args[1]) - 1;
        printf("Argument : %d\n", req->arg1);
    }
    else
    {
        printf("Erreur sur la commande\n");
    }
    return args;
}

int server_manager(int clientfd, rio_t rio, char *buf)
{
    REQ_MSG req = (REQ_MSG)malloc(sizeof(REQ_MSG));
    REP_MSG rep = (REP_MSG)malloc(sizeof(REP_MSG));
    char **args = create_request(buf, req);
    char rep_content[MAXLINE];
    int fd;
    rio_t file_rio;

    Rio_writen(clientfd, req, sizeof(REQ_MSG));
    Rio_writen(clientfd, args[1], req->arg1);

    if (Rio_readnb(&rio, rep, sizeof(REP_MSG)) > 0)
    {
        printf("Response content : \n\tstatus : %d\n\tsize : %d\n", rep->state, rep->size);
        if (rep->state)
        {
            char file_location[sizeof(FILE_LOCATION) + MAXLINE] = FILE_LOCATION;
            args[1][len(args[1]) - 1] = 0;
            strcat(file_location, args[1]);

            fd = open(file_location, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            Rio_readinitb(&file_rio, fd);
            time_t start = time(NULL);
            for (int i = 0; i < (rep->size / MAXLINE) + 1; i++)
            {
                if (i == (rep->size / MAXLINE))
                {
                    if (MAXLINE / rep->size != 0)
                    {
                        printf("Fin de fichier: %d\n", rep->size % MAXLINE);
                        Rio_readnb(&rio, rep_content, rep->size % MAXLINE);
                        save_content(fd, rep_content, rep->size % MAXLINE);
                    }
                }

                else
                {
                    printf("Lecture de fichier\n");
                    Rio_readnb(&rio, rep_content, MAXLINE);
                    save_content(fd, rep_content, MAXLINE);
                }
            }
            time_t end = time(NULL);
            Close(fd);

            printf("File %s saved at %s (%d s)\n", args[1], FILE_LOCATION, end - start);
        }
        else
        {
            Rio_readnb(&rio, rep_content, rep->size);
            printf("Server error : %s\n", rep_content);
        }
        return 0;
    }
    else
    { /* the server has prematurely closed the connection */
        printf("The server closed the connection\n");
        return -1;
    }
}
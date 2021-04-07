#include "gestionner.h"
#include <string.h>
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
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

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

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

int len(char* string){
    int nb = 0;
    while (*string)
    {
        nb++;
        *string++;
    }
    return nb;
}

void save_file(char* name, char* content){
    char file_location[sizeof(FILE_LOCATION) + MAXLINE] = FILE_LOCATION;
    name[len(name) - 1] = 0;
    strcat(file_location,name);
    printf("Path : %s\n",file_location);
    FILE* f = fopen(file_location,"w");
    fprintf(f,content);
    fclose(f);
}

char ** create_request(char* buf, REQ_MSG req){
    char** args = str_split(buf,' ');
    for(int i = 0; *(args +i); i++){
        printf("[%d] => %s\n",i,*(args + i));
    }
    if(strcmp(args[0],"get") == 0){
        req->cmd = 1;
        req->arg1 = len(args[1]) - 1;
        printf("Argument : %d\n",req->arg1);
    }
    else {
        printf("Erreur sur la commande\n");
    }
    return args;
}

int server_gestionner(int clientfd,rio_t rio, char *buf){
    REQ_MSG req = malloc(sizeof(REQ_MSG));
    REP_MSG rep = malloc(sizeof(REP_MSG));
    char ** args = create_request(buf,req);
    char rep_content[MAXLINE];
    Rio_writen(clientfd,req,sizeof(REQ_MSG));
    Rio_writen(clientfd,args[1],req->arg1);

    if (Rio_readnb(&rio, rep, sizeof(REP_MSG)) > 0) {
        printf("Response content : \n\tstatus : %d\n\tsize : %d\n",rep->state,rep->size);
        if(rep->state){
            Rio_readnb(&rio,rep_content,rep->size);
            save_file(args[1],rep_content);
            printf("File %s saved at %s\n",args[1],FILE_LOCATION);
        }
        else{
            Rio_readnb(&rio,rep_content,rep->size);
            printf("Server error : %s\n",rep_content);
        }
        return 0;
    }
    else { /* the server has prematurely closed the connection */
        printf("The server closed the connection\n");
        return -1;
    }
}
#include "gestionner.h"

int response_gestionner(rio_t rio){
    char buf[MAXLINE];
    if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
        Fputs("Server header : ",stdout);
        Fputs(buf, stdout);
        Fputs("\n",stdout);
        if(Rio_readlineb(&rio, buf, MAXLINE) > 0){
            Fputs("Server reponse : ",stdout);
            Fputs(buf, stdout);
            Fputs("\n",stdout);
        } 
        else{
            printf("Failed to get server content\n");
            return -2;
        }
       
        return 0;
    }
    else { /* the server has prematurely closed the connection */
        printf("The server closed the connection\n");
        return -1;
    }
}
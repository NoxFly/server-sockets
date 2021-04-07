#ifndef GESTIONNER_H
#define GESTIONNER_H

#include "csapp.h"

#define HEADER_SIZE 9
#define FILE_LOCATION "storage/"

typedef struct REQ_MSG *REQ_MSG;

struct REQ_MSG{
    int cmd;
    int arg1;
};

typedef struct REP_MSG *REP_MSG;

struct REP_MSG{
    int state;
    int size;
};

int server_gestionner(int clientfd,rio_t rio, char* buf);

#endif // !GESTIONNER_H
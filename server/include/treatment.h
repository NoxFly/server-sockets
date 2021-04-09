#ifndef TREATMENT_H
#define TREATMENT_H

#define STORAGE_LOCATION "server/storage/"
#define HEADER_SIZE 8

#define T_FILE_OPEN 0
#define T_FILE_NOPEN 1


typedef struct REQ_MSG *REQ_MSG;

struct REQ_MSG{
    int cmd;
    int arg1;
};

typedef struct REP_MSG *REP_MSG;

struct REP_MSG{
    int state;
    long long size;
};

void command_treatment(int connfd);

#endif
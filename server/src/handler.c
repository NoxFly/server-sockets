#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
void exit_handler_child(int sig){
    printf("Child exited\n");
    exit(0);
}

void exit_handler_main(int sig){
    printf("Main exited\n");
    kill(0,SIGINT);
    exit(0);
}
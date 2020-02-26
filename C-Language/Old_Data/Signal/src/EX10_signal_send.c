#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc != 2){
        fprintf(stderr,"usage:%s pid\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t pid = atoi(argv[1]);    
   // sleep(2);
    union sigval mysigval;
   mysigval.sival_int = 10900;
/*
    char *str = "hello henry";
    char data[10];
    memset(data,0,sizeof(data));
    data[0]='1';
    data[1]='2';
    data[2]='3';
    data[3]='4';  
    data[4]='5';
    

    mysigval.sival_ptr = data;
*/
    printf("sending SIGINT signal to %d......\n",pid);
    if(sigqueue(pid,SIGINT,mysigval) == -1){
        perror("sigqueue error");
        exit(EXIT_FAILURE);
    }
    return 0;
}

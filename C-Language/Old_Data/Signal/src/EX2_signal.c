#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

void sig_handler(int signo);
int main(void)
{
    printf("main is waiting for a signal\n");
    __sighandler_t prehandler;
    prehandler = signal(SIGINT,SIG_DFL);
    if(prehandler == SIG_ERR){
        perror("signal errror");
        exit(EXIT_FAILURE);
    }

    for(; ;);//有时间让我们发送信号


    return 0;
}

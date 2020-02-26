#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void sig_handler(int signo);
int main(void)
{
    printf("mian is waiting for a signal\n");
    if(signal(SIGINT,sig_handler) == SIG_ERR){
        perror("signal errror");
        exit(EXIT_FAILURE);
    }
    printf("useing raise to send a signal to himself\n");
    raise(SIGINT);
    sleep(1);
    printf("useing kill to send a signal to himself\n");
    kill(getpid(),SIGINT);
    
    return 0;
}

void sig_handler(int signo)
{
    printf("catch the signal SIGINT %d\n",signo);
}

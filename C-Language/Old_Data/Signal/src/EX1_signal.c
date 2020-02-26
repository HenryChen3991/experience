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
    if(signal(SIGQUIT,sig_handler) == SIG_ERR){
        perror("signal errror");
        exit(EXIT_FAILURE);
    }

    for(; ;);//有时间让我们发送信号


    return 0;
}

void sig_handler(int signo)
{
    switch(signo){
	case 2:
    		printf("catch the signal SIGINT ( Ctrl + C ) %d\n",signo);
		break;
	case 3:
		printf("catch the signal SIGQUIT ( Ctrl + \\ ) %d\n",signo);
		break;
    }
}

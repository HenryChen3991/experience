#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


#define ERR_EXIT(m) \
    do \
    { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

void handler(int sig);
int main(int argc, char *argv[])
{
    if (signal(SIGUSR1, handler) == SIG_ERR)
        ERR_EXIT("signal error");
	if (signal(SIGINT, handler) == SIG_ERR)
        ERR_EXIT("signal error");
    pid_t pid = fork();
    if (pid == -1)
        ERR_EXIT("fork error");

    if (pid == 0)
    {
        sleep(1);
        kill(getppid(), SIGUSR1);
		kill(getppid(), SIGINT);
        exit(EXIT_SUCCESS);
    }
	
    int n = 5;
    do
    {
        printf("the number of  seconds  left to sleep is %d s\n",n);
		//kill(0, SIGINT);
        n = sleep(n);
    } while (n > 0);
    return 0;
}

void handler(int sig)
{
    printf("recv a sig=%d\n", sig);
}

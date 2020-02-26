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
void printsigset(sigset_t *set)
{
    int i;
    for (i=1; i<NSIG; ++i)
    {
        if (sigismember(set, i))
            putchar('1');
        else
            putchar('0');
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    sigset_t pset;
    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
    if (signal(SIGINT, handler) == SIG_ERR)
        ERR_EXIT("signal error");
    if (signal(SIGQUIT, handler) == SIG_ERR)
        ERR_EXIT("signal error");

    sigprocmask(SIG_BLOCK, &bset, NULL);//将信号加入进程阻塞集中
    for (;;)
    {
        sigpending(&pset);
        printsigset(&pset);
        sleep(1);
    }
    return 0;
}

void handler(int sig)
{
    if (sig == SIGINT)
        printf("recv a sig=%d\n", sig);
    else if (sig == SIGQUIT)
    {
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset, SIGINT);
        sigprocmask(SIG_UNBLOCK, &uset, NULL);
    }
}

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <unistd.h>

/* 版本1, 可靠信号将被递送多次 */
#define MYSIGNAL SIGRTMIN+5
/* 版本2, 不可靠信号只被递送一次 */
//#define MYSIGNAL SIGTERM

void sig_handler(int signum)
{
    psignal(signum, "catch a signal");
}

int main(int argc, char **argv)
{
    sigset_t block, pending;
    int sig, flag;

    /* 设置信号的handler */
    signal(MYSIGNAL, sig_handler);

    /* 屏蔽此信号 */
    sigemptyset(&block);
    sigaddset(&block, MYSIGNAL);
    printf("block signal\n");
    sigprocmask(SIG_BLOCK, &block, NULL);

    /* 发两次信号, 看信号将会被触发多少次 */
    printf("---> send a signal --->\n");
    kill(getpid(), MYSIGNAL);
    printf("---> send a signal --->\n");
    kill(getpid(), MYSIGNAL);

    /* 检查当前的未决信号 */
    flag = 0;
    sigpending(&pending);
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(&pending, sig)) {
            flag = 1;
            psignal(sig, "this signal is pending");
        } 
    }
    if (flag == 0) {
        printf("no pending signal\n");
    }

    /* 解除此信号的屏蔽, 未决信号将被递送 */
    printf("unblock signal\n");
    sigprocmask(SIG_UNBLOCK, &block, NULL);

    /* 再次检查未决信号 */
    flag = 0;
    sigpending(&pending);
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(&pending, sig)) {
            flag = 1;
            psignal(sig, "a pending signal");
        } 
    }
    if (flag == 0) {
        printf("no pending signal\n");
    }

    return 0;
}

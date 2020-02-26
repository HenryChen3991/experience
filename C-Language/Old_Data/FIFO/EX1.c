/*  > file name: process_.c
    > Author: Simba
    > Mail: dameng34@163.com
    > Created Time: Sat 23 Feb 2013 02:34:02 PM CST
 ************************************************************************/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

int main(int argc, char *argv[])
{
    printf("mkfifo tp\n");
    mkfifo("tp", 0777);
    printf("open makefile\n");
    FILE* infd = fopen("Makefile", "r");
    if (infd == NULL)
        ERR_EXIT("open error");
    else
        printf("open makefile success\n");
    FILE *outfd;
    printf("open tp\n");
    outfd = fopen("tp", "w+");
    if (outfd == NULL){
        ERR_EXIT("open error");
    }
    else
        printf("open tp success\n");
    char buf[1024];
    int n;
    while ((n = fread(buf, sizeof(char), 1024,infd)) > 0){
        fwrite(buf,sizeof(char), n,outfd);
        printf("read/write\n");
    }

   fclose(infd);
   fclose(outfd);

    return 0;
}

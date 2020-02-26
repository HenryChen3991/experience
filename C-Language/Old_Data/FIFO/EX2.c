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
    while(1){
      system("cat tp");
    }
    FILE* outfd = fopen("Makefile2", "w+");
    if (outfd == NULL)
        ERR_EXIT("open error");
    else
        printf("open makefile2 success\n");
    
    FILE* infd;
    infd = fopen("tp", "r+");
    if (infd == NULL)
        ERR_EXIT("open error");
    else
        printf("open tp success\n");

    char buf[1024];
    int n;
    printf("start to copy\n");
    while ((n = fread(buf, sizeof(char), 1024,infd)) > 0){
        fwrite(buf,sizeof(char), n,outfd);
        printf("read/write\n");
    }

    fclose(infd);
    fclose(outfd);
    unlink("tp"); // delete a name and possibly the file it refers to
    return 0;
}

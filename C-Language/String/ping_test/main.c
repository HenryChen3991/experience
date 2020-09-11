#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
char ping_server_list[5][64] = { {"8.8.8.8"},{"heartbeat.belkin.com"},{"www.belkin.com"},{"223.5.5.5"},{"115.42.228.246"}};
#define PING_SERVER_LIST_SIZE (sizeof(ping_server_list)/sizeof(ping_server_list[0]))
static int check_ping_server(void)
{
    FILE *fp;
    unsigned int ping_fail=1;
    int i = 0;
    char cmd[128]={0};
    for(i=0;i<PING_SERVER_LIST_SIZE;i++){
        memset(cmd,0,sizeof(cmd));
        snprintf(cmd,sizeof(cmd),"ping %s -c 1 -W 1 -q > /dev/null 2>&1 && echo $?",ping_server_list[i]);
        printf("cmd = %s\n",cmd);
        if ((fp = popen(cmd, "r")) == NULL) {
            printf("ping process error\n");
            return 0;
        }
        fscanf(fp, "%u", &ping_fail);
        printf("ping %s fail: %d\n", ping_server_list[i], ping_fail);
        if (!ping_fail) {
            return 1;
        }
    }
    return 0;
}
void main(void)
{
     check_ping_server();
}

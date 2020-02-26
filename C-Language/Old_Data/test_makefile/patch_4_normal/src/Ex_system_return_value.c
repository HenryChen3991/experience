#include <stdio.h>
#include <stdlib.h>

void main(void)
{
 int status;

 status = system("ps ax | grep 'ssyslogd' | grep -v 'grep'");
 if(status == -1)
	printf("system error\n");
 else
	printf("succeed\n");
}

#include <stdio.h>
#include <stdlib.h>
typedef enum{
	LOG_STDERR = 1,
	LOG_SYSLOG = 2,
	LOG_TELNET = 3
}CmsLog;


void main(void)
{
	CmsLog lo;
        lo = LOG_STDERR;
	printf("lo = %d\n",lo);

}


#include <stdio.h>
#include <sys/types.h>
#define DBG(msg,arg...) \
	printf("%s:%s(%d):" msg,__FILE__,__FUNCTION__,__LINE__,##arg)


int main (void)
{
    printf("line: %d \n", __LINE__);
    printf("file: %s \n", __FILE__);
	printf("function: %s \n", __FUNCTION__);
	printf("function: %s \n", __func__);
	printf("time: %s \n", __TIME__);
	printf("date: %s \n", __DATE__);
	printf("STDC: %d \n", __STDC__);
	printf("STDC_HOSTED: %d \n", __STDC_HOSTED__);
	printf("VERSION: %s \n", __VERSION__);
	printf("TIMESTAMP: %s \n", __TIMESTAMP__);
	int a = 3;
	char *str = "hello";
	
	DBG("str = %s, a = %d\n",str,a);
    return 0;
}

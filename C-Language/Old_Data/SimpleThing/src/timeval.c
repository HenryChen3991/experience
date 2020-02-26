
#include<stdio.h>
#include <stdlib.h>
#include <time.h>  
#include <sys/time.h>
int main()
{
	struct timeval tpstart,tpend;
    	int timeuse;
	srand(10);
	printf("生成150000个随机数：\n");
	//获得初始时间
	gettimeofday(&tpstart,NULL);
	for (int i=0; i<150000; i++)
	{
		rand()%100;
   	// printf("%d ", rand()%100);
	}
	//获取结束时间
	gettimeofday(&tpend,NULL);
	printf("\n");
	//计算耗时
	timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
	printf( "RunningTime:\n%d微秒\n",timeuse);
 

	printf("------------------------------------------------------------\n");
	struct timeval tv;
    	struct timezone tz;
    	gettimeofday (&tv, &tz);
   	printf("tv_sec; %ld\n", tv.tv_sec);
    	printf("tv_usec; %ld\n", tv.tv_usec);
    	printf("tz_minuteswest; %d\n", tz.tz_minuteswest);
    	printf("tz_dsttime, %d\n", tz.tz_dsttime);


}

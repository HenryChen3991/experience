#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int *ctx = NULL;
void main(void)
{
	struct timeval start_time = {0}, end_time = {0};
	long long delta = 0;
	gettimeofday(&start_time, NULL);
	do{
		ctx = calloc(1, sizeof(int));
		if (ctx){
			if (1) {
				free(ctx);
				ctx = NULL;
				gettimeofday(&end_time, NULL);
				delta = (end_time.tv_sec -  start_time.tv_sec) * 1000LL + (end_time.tv_usec - start_time.tv_usec) / 1000;
				printf("delta = %lld\n",delta);
				usleep(100000);
			}
			else
				break;
		}
	}while(delta<1000);
	
}

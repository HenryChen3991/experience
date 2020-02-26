#include <stdio.h>
#include<sys/time.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>

#define AesKeyBytes     (128 / 8)
int generateRand(void)
{
	
	return rand();
}

static void GenerateKey(int *key, int mode)
{
	char i;
	struct timeval Time; 
	printf("-----------------------------------------\n");
	switch(mode)
	{
		case 0:
			printf("mode = time(NULL)\n");
			srand(time(NULL));
//			printf("-----------------------------------------\n");
			break;
		case 1:
	//		struct timeval Time; 
			printf("mode = (time.tv_sec * 1000) + (time.tv_usec / 1000)\n");
			gettimeofday(&Time,NULL);
			printf("(Time.tv_sec * 1) = %lu\t (Time.tv_usec / 1) =%lu\t (Total = %lu)\n",(Time.tv_sec * 1),(Time.tv_usec / 1),(Time.tv_sec * 1)+(Time.tv_usec / 1));
		//	printf("(Time.tv_sec * 1000) + (Time.tv_usec / 1000) = %lu\n",(Time.tv_sec * 1000) + (Time.tv_usec / 1000));
			srand((Time.tv_sec * 1) + (Time.tv_usec / 1));
//			printf("-----------------------------------------\n");
			break;
	}

	for(i = 0; i < (AesKeyBytes/sizeof(int)) ; i++)
	{
		key[i] = ((int)generateRand() << 16) | generateRand();
	}
	printf("pid = %d\n",getpid());
	printf("key = 0x%x\n",key[0]);
        printf("key = 0x%x\n",key[1]);
        printf("key = 0x%x\n",key[2]);
        printf("key = 0x%x\n",key[3]);
//	printf("-----------------------------------------\n");
}
void main(void)
{
	int key[AesKeyBytes/sizeof(int)];
	int i=0;
//	GenerateKey(key,0);
	GenerateKey(key,1);
		


}

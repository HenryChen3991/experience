#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
	int *ptr = (int *)malloc(sizeof(int)*3);
	(ptr[0]) = 1;
	(ptr[1]) = 2;
	(ptr[2]) = 3;
	int i =0;
	for(i = 0; i < 3 ;i++)
		printf("%d\n",(ptr[i]));
	*ptr = 5;
	*(ptr+1) = 6;
	*(ptr+2) = 7;
	for(i = 0; i < 3 ;i++)
                printf("%d\n",(ptr[i]));

    return 0;
}



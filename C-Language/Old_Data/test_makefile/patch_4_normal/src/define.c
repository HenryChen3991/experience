#include<stdlib.h>
#include<stdio.h>

#define a 1
#define b 12
#define c a+b+1

void main(void)
{

	printf("c = %d\n",c);
	unsigned char str2[50];
	printf("size = %d\n",sizeof(str2));
	unsigned char str[50-c];
	printf("size = %d\n",sizeof(str));
}

#include<stdio.h>
#include<stdlib.h>
void callfun(int b)
{
        printf("11\n");
}
int main(int argc,char **argv)
{
		char *str = "aaa";
		callfun(str);

	unsigned int aa = 0x12345678;
	int i=0;
	printf("i = %s ,aa = 0x%x , aa+1 = 0x%x\n",i,aa,aa+1);
	unsigned int *b = &aa;
	printf("b = 0x%x\n",*b);

        char *s = "00010002";
	unsigned int *c = s;
	printf("c = 0x%x\n",*c);
	printf("c = 0x%x\n",*(c+1));



	int key = 100;
	printf("key = 0x%08x\n",key);




        return 0;
}

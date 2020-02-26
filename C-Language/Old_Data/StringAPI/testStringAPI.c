#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
        int len = 20;
        char a[] = "hello, abc";
        char b[] = "hello, cba";
        printf("a = %s\n",a);
        printf("b = %s\n",b);
        if ( strncmp(a,b,len)==0)
                printf("equal\n");
        else
		printf("not equal\n");
	return 0;
}


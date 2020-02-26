#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv)
{
	char* a[]={"china","french","america"};
	int i=0;
	
	for(i=0;i<3;i++)
		printf("%s\n",a[i]);
	
	printf("*******************\n");
	char** s;
	s=a;
	for(i=0;i<3;i++)
		printf("%s\n",s[i]);
	
	printf("*******************\n");
	for(i=0;i<3;i++)
		printf("%s\n",*s++);

	printf("*******************\n");

    printf("sizeof(a) = %d\n",sizeof(a));
	printf("a length = %d\n",sizeof(a)/sizeof(char *));
	
    return 0;
}



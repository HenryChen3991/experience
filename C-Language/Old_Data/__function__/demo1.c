#include <stdio.h>
#include <stdlib.h>
int addTwoNum(int a ,int b)
{
	printf("%s\n",__FUNCTION__);
	return a+b;
}
int main(int argc,char **argv)
{
        int i=0;
        printf("argc = %d\n",argc);
	for (i=0;i<argc;i++)
                printf("argv[%d]=%s\n",i,argv[i]);
 	int c = addTwoNum(3,4); 
       return 0;
}

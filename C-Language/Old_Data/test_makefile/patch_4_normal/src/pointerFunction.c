#include<stdio.h>
#include<stdlib.h>


int addTwoNum(int a,int b)
{
        return a+b;
}
int call_a_func(int (*pf)(int,int))
{
        return pf(3,5);
}

int main(int argc,char **argv)
{
        printf("%d\n",call_a_func(addTwoNum));
	printf("end\n");
        return 0;
}

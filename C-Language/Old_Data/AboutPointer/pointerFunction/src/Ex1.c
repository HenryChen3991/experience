#include<stdio.h>
#include<stdlib.h>


int addTwoNum(int a,int b)
{
        return a+b;
}
int (*pf_test)(int a,int b) = addTwoNum;
int call_a_func(int (*pf)(int,int))
{
        return pf(3,5);
}

int main(int argc,char **argv)
{
        printf("%d\n",call_a_func(addTwoNum));
	printf("%d\n",pf_test(3,4));
        return 0;
}

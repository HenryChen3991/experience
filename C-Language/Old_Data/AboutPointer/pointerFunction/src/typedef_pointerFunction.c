#include<stdio.h>
#include<stdlib.h>

typedef int (*tpf)(int x ,int y);

int addTwoNum(int a,int b)
{
        return a+b;
}
/*
int call_a_func(int (*pf)(int,int))
{
        return pf(3,5);
}
*/
int main(int argc,char **argv)
{
		tpf add2 = NULL;
		add2 = addTwoNum;		
        printf("%d\n",add2(3,4));
        return 0;
}

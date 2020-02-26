#include<stdio.h>
#include<stdlib.h>
typedef int (*tpf)(int x ,int y);
typedef struct
{
		tpf add;
		tpf sub;
}tpfunc;
int addTwo(int a,int b)
{
        return a+b;
}
int subTwo(int a,int b)
{
		return a-b;
}
void init(tpfunc *tp)
{
	tp->add = addTwo;
	tp->sub = subTwo;
}
//int (*func)(int,int)
int call_func(tpf func,int a,int b)
{
        return func(a,b);
}

int main(int argc,char **argv)
{
		tpfunc MathFunc;
		init(&MathFunc);	
        printf("%d\n",MathFunc.add(3,4));
        return 0;
}

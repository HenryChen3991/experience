#include <stdio.h>
#include <stdlib.h>

//error will cause extra ;
//#define foo(x) bar(x); qoo(x);

//correct
#define foo(x) do { bar(x);qoo(x);} while(0)

#define bar(x) x
#define qoo(x) x
void main(void){
	if(1)
		foo(1);
	else
		printf("in else\n");

}

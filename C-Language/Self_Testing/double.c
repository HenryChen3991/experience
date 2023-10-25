#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int a=-1;
void aaa()
{
	if(a != 1 && a!= 3) 
		printf("a = %d\n",a);
	else{
		printf("xx\n");
		return ;
	}
	printf("+++++\n");
}

void main(void)
{

	double i = 5111222333444555;
	printf("i = %.0lf\n",i);
/*	while(1){
		aaa();
		a++;
		sleep(2);
	}
*/
	int b=-1;
	if(b)
		printf("bbb\n");
}

#include<stdio.h>
#include<stdlib.h>
void callfun(int b)
{
        printf("b = %d\n",b);
}
int main(int argc,char **argv)
{
        int a = 3;
        printf("a = %d\n",a);
        callfun(a-1);
        printf("a = %d\n",a);

        return 0;
}

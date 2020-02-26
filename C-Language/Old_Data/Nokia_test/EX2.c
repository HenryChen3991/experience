#include <stdio.h>
#include <stdlib.h>

int parsexdigit(char str)
{
    int digit;

    if ((str >= '0') && (str <= '9')) digit = str - '0';
    else if ((str >= 'a') && (str <= 'f')) digit = str - 'a' + 10;
    else if ((str >= 'A') && (str <= 'F')) digit = str - 'A' + 10;
    else return -1;

    return digit;
}

void main(void)
{
    char a = '5';
    char b = 'a';
    int a1 = parsexdigit(a);
    int b1 = parsexdigit(b);
    printf("a1 = %d , b1 = %d\n",a1,b1);
    printf("a1 << 8 = 0x%x\n",(a1<<8));
    printf("a1 << 4 = 0x%x\n",(a1<<4));
    printf("b1 = 0x%x\n",b1);
    char c = (a1<<4) | b1;
    printf("c = 0x%x\n",c);

}

#include <unistd.h>
#include <stdio.h>
int main()
{
    if(access("Makefile", R_OK) == 0)
    printf("Makefile can be read\n");
}

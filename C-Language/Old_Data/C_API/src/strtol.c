#include<stdlib.h>
#include <stdio.h>

main(int argc, char *argv[])
{
    char a[] = "10000";

    printf("a = %ld\n",strtol(a,NULL,10));
    printf("a = 0x%lx\n",strtol(a,NULL,0));
   
    int b = 0;
    b = strtol(a,NULL,10);
    printf("%s\n",argv[argc-1]);
    unsigned char c = strtol( argv[argc-1],NULL,0);
    printf("%c 0x%x\n",c,c);
    printf("%d\n",b);
}

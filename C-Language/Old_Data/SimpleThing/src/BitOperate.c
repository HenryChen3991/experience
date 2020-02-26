#include <stdio.h>
#include <stdlib.h>

void main(void)
{
    unsigned short bitMask = 0;
    bitMask |= (0x1<<16-3);  //0x2000
    printf("bitMask = 0x%x\n",bitMask);


    bitMask = 0;
    bitMask |= (0x1<<16-1);  //0x8000
    printf("bitMask = 0x%x\n",bitMask);

}

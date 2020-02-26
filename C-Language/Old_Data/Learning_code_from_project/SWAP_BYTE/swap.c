#include<stdio.h>
#include<stdlib.h>
#define ENDIAN_SWAP32(value) \
         ((((unsigned long)((value) & 0x000000FF)) << 24) | \
           (((unsigned long)((value) & 0x0000FF00)) << 8) | \
           (((unsigned long)((value) & 0x00FF0000)) >> 8) | \
           (((unsigned long)((value) & 0xFF000000)) >> 24))
void main(void)
{
	unsigned int a = 0xffffffef;
	printf("a = 0x%lx\n",a);
	printf("Manual = %d.%d.%d.%d\n", (a>>24)&0x000000ff ,(a>>16)&0x000000ff, (a>>8)&0x000000ff ,(a)&0x000000ff);
	printf("ENDIAN_SWAP32 = 0x%x\n",ENDIAN_SWAP32(a));

}

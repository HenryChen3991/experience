#include <stdio.h>

void main(void)
{
	unsigned short meid = 0;
	if( meid == 0)
		printf("meid = %d\n",meid);

        unsigned char id = 0;
	if( id == 0 )
		printf("id == 0, id = 0x%x\n",id);
	if( id == 0x30 )
		printf("id == 0x30\n");


	unsigned char id2 = 1;
	printf("id2 = 0x%x  , %c\n",id2,id2);  //can't use %c to show


	unsigned char i3 = 31;
	printf("i3 = 0x%x\n",i3);
        printf("i3 = %u\n",i3);
	printf("i3 = %c\n",i3);
	
	unsigned char i4 = 0x1f;
	if( i4 == i3)
		printf("i4 == i3\n");
}

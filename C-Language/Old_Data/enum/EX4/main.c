#include <stdio.h>
#include <stdlib.h>
#include "Manager.h"
#include <string.h>


void check(char *str)
{
  printf("Henry-- str = %2hhx:%2hhx:%2hhx\n",*str,*(str+1),*(str+2));
  //printf("Henry-- str = %2hhx:%2hhx:%2hhx\n",str,(str+1),(str+2)); //error
}
void main(void){
	char mac_src[3] = { 0xf4,0xd6,0xa0 };
	char t_src[6] = { 0xf4,0xd6,0xa0,0xf4,0xd6,0xa0 };
//	char t_src[6] = { 0x2c,0xfa,0xa2,0xf4,0xd6,0xa0 };
//	char t_src[6] = { 0x00,0x00,0xa2,0xf4,0xd6,0xa0 };
//	printf("Henry-- src %2hhx:%2hhx:%2hhx\n",mac_src[0],mac_src[1],mac_src[2]);
//	printf("Henry-- tar %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx\n",t_src[0],t_src[1],t_src[2],t_src[3],t_src[4],t_src[5]);



//	check( t_src+3);


//	printf("OLT_TYPE = %d\n",getOltType());


//	check( R_OltPrototype.r_olt[0].olt_mac );
	new_check(t_src);
//	printf("OLT_TYPE = %d\n",getOltType());

}



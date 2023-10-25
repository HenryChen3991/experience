#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(void)
{
   char str[] = "macAddr=AA:BB:CC:DD:11:55";
   char s[20];
   char s1[20];

   sscanf(str,"%[^=]=%s",s,s1);
   printf("str : [  %s ]\n",s);
   printf("s1 = %s\n",s1);
 

   unsigned char pkt_dst_mac_addr[17+1]={0};
   unsigned char pkt_data[] = {0x00,0xe0,0x92,0x11,0x11,0x1d};
   printf("%02x %02x\n",pkt_data[0],pkt_data[1]);
   snprintf(pkt_dst_mac_addr,sizeof(pkt_dst_mac_addr),"%02x:%02x:%02x:%02x:%02x:%02x",pkt_data[0],pkt_data[1],pkt_data[2],pkt_data[3],pkt_data[4],pkt_data[5]);
   printf("%s\n",pkt_dst_mac_addr);
   if(strncmp(pkt_dst_mac_addr, "00:e0:92:11:11:1d", strlen(pkt_dst_mac_addr))==0){
		printf("### equal ###\n");
	}
	else
		printf("### not equal ###\n");
}

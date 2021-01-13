#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"


void handle_abbrev_ipv6_addr(char *ipv6_addr,char *abbrev_ipv6_addr){
	DEBUG_COLOR(LIGHT_CYAN,"input : %s",ipv6_addr);
	char *token = NULL;
	//char new[128]={0};
	token = strstr(ipv6_addr,"0000");
	if(token!=NULL){
		if((token-ipv6_addr)==0){
			strcat(abbrev_ipv6_addr,":");
		}else{
			memcpy(abbrev_ipv6_addr,ipv6_addr,(token-ipv6_addr));
		}
		while(strncmp(token,"0000",4)==0){
			token += 5;
		}
		if((token-1)[0]=='/'){
			strcat(abbrev_ipv6_addr,":");
		}
		strcat(abbrev_ipv6_addr,token-1);
		token = strstr(abbrev_ipv6_addr,"/");
		if(token!=NULL){
			abbrev_ipv6_addr[(token-abbrev_ipv6_addr)]='\0';
		}
	}
	else{
		token = strstr(ipv6_addr,"/");
		if(token!=NULL){
			ipv6_addr[(token-ipv6_addr)]='\0';
		}
		strcpy(abbrev_ipv6_addr,ipv6_addr);
	}
}


void main(void)
{
    char ipv6_addr[]="0100:0200:0000:0000:C641:1EFF:0000:0000/64";
    char abbrev_ipv6_addr[128]={0};
    handle_abbrev_ipv6_addr(ipv6_addr,abbrev_ipv6_addr);
    DEBUG_COLOR(LIGHT_CYAN,"output: %s",abbrev_ipv6_addr);
}

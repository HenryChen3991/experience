#include <stdio.h>
#include <stdlib.h>
#include "dumpHex.h"
#include "openssl.h"
#include "debug.h"
#include <unistd.h>
#include <string.h>

void usage()
{
	printf("Welcome to Poepn + Openssl Example\n");
	printf("Usage:\n");
	printf("\t./main <FILE> [encrypt/decrypt]\n");
	printf("\tIt will encrypt <FILE> to <FILE>.lock\n");
	exit(1);
}


openssl_action_e analyze_parameter(int argc,char *argv[],char **file)
{
	int i = 0;
	char *action = NULL;
	openssl_action_e ret = not_supported;

	if(argc==3){

#if 0 
	/*Method 1*/
		//So, as others also explained you have to know the length of the string you want to allocate
		//, and then add 1 to store the terminal \0, C implicitly use to put at the end of strings.
		*file = (char*)malloc(strlen(argv[1])+1);
		snprintf(*file,strlen(argv[1])+1,"%s",argv[1]);
#endif //0

	/*Method 2*/
		//stdrup可以直接把要復制的內容復制給沒有初始化的指針，因為它會自動分配空間給目的指針
		//最後必須釋放內存
		*file = strdup(argv[1]);
		action = argv[2];

#if DEBUG_FLAG
		DEBUG("file = %s, action = %s",*file,action);
		for(i=0;i<argc;i++)
			DEBUG("argv[%d] = %s",i,argv[i]);
#endif //DEBUG_FLAG

		if( access(*file,R_OK)== CMSRET_SUCCESS ){
			if( (ret = analyze_openssl_action(action)) != not_supported ){
				return ret;
			}
			else
				DEBUG("%s not support",action);
		}
		else{
			DEBUG("%s not exist",*file);
		}
	}
	usage();
}

void main(int argc,char *argv[])
{
	FILE *fp = NULL;
	openssl_action_e openssl_action = not_supported;
	unsigned char buffer[2000]={0};
	char *file = NULL;
	openssl_action = analyze_parameter(argc,argv,&file);
#if DEBUG_FLAG
	DEBUG("file = %s",file);
	DEBUG("openssl_action = %s",openssl_action_name[openssl_action]);
#endif //DEBUG_FLAG
	openssl_aes_128_cbc_encrypt("certificatehost");
	//fp = popen("openssl aes-128-cbc -salt -in certificatehost -out certificatehost.lock -k gemtek","r");
	fp = popen("openssl aes-128-cbc -d -salt -in certificatehost.lock -k gemtek","r");
	if( fp == NULL)
	{
		printf("cmd could not be execute\n");
		exit(1);
	}
	int fp_len;
	fp_len = fread(buffer,sizeof(char),sizeof(buffer),fp);
	dump_hex_api(buffer,fp_len);
	pclose(fp);
	openssl_aes_128_cbc_decrypt("certificatehost");

	free(file);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "openssl.h"

#define SEED "gemtek"

char *openssl_action_name[32]=
{
	"not_supported",
	"encrypt",
	"decrypt"
};

void verify_hash(char *str);

openssl_action_e analyze_openssl_action(char *action)
{
	if(strncmp(action,"encrypt",sizeof("encrypt")) == CMSRET_SUCCESS){
		return encrypt;
	}
	else if(strncmp(action,"decrypt",sizeof("decrypt")) == CMSRET_SUCCESS){
		return decrypt;
	}
	else
		return not_supported;
}

void openssl_aes_128_cbc_encrypt(char *file_path)
{
	char cmd[256]={0};
#if DEBUG_FLAG
	DEBUG("");
#endif //DEBUG_FLAG
	snprintf(cmd,sizeof(cmd),"openssl aes-128-cbc -salt -in %s -out %s.lock -k %s",file_path,file_path,SEED);
	system(cmd);
}

void openssl_aes_128_cbc_decrypt(char *file_path)
{
	char cmd[256]={0};
#if DEBUG_FLAG
	DEBUG("");
#endif //DEBUG_FLAG
	snprintf(cmd,sizeof(cmd),"openssl aes-128-cbc -d -salt -in %s.lock -out %s.unlock -k %s",file_path,file_path,SEED);
	system(cmd);
	verify_hash(file_path);
}

void verify_hash(char *str)
{
	FILE *fp=NULL;
	char cmd[256]={0};
	char hash_1[512]={0};
	char hash_2[512]={0};
	char hash_3[512]={0};

	snprintf(cmd,sizeof(cmd),"openssl aes-128-cbc -d -salt -in %s.lock -k %s | sha512sum | cut -d \" \" -f 1",str,SEED);
	fp = popen(cmd,"r");
	if(fp != NULL){
		fread(hash_1,sizeof(char),sizeof(hash_1),fp);
		printf("Decrypt %s.lock:\n%s",str,hash_1);
		pclose(fp);
	}
	
	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"sha512sum %s | cut -d \" \" -f 1",str);
	fp = popen(cmd,"r");
	if(fp != NULL){
		fread(hash_2,sizeof(char),sizeof(hash_2),fp);
		printf("%s:\n%s",str,hash_2);
		pclose(fp);
	}

	memset(cmd,0,sizeof(cmd));
	snprintf(cmd,sizeof(cmd),"sha512sum %s.unlock | cut -d \" \" -f 1",str);
	fp = popen(cmd,"r");
	if(fp != NULL){
		fread(hash_3,sizeof(char),sizeof(hash_3),fp);
		printf("%s.unlock:\n%s",str,hash_3);
		pclose(fp);
	}

	if( strncmp(hash_1,hash_2,sizeof(hash_1)) == 0 &&
	    strncmp(hash_2,hash_3,sizeof(hash_2)) == 0 ){
		printf("FILE: %s.unlock,%s's hash value are same\n",str,str);
	}
	else{
		printf("FILE: %s.unlock,%s's hash value are not same, haing some error!!!\n",str,str);
	}
}

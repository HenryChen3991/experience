#include <stdio.h>
#include <string.h>
#include "aes.h"
 
 
int main(void)
{
	unsigned char key[] = { /* 加密的 KEY */
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	};
	unsigned char iv[] = { /* Initial vector */
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
	};
	unsigned char buf[1024*128];
	FILE *fp, *fp2;
	int i, ret, c;
	unsigned char empty[16];
 
#ifndef AES256 /* 此範例只允許 AES256 */
//    #error "Need to use AES256"
#endif
	{ // Do encryption 
    	struct AES_ctx ctx; /* 一次將檔案全讀進來，方便展示 */
		fp=fopen("aes.c", "r");
		ret = fread(buf, 1, sizeof(buf), fp);
		fclose(fp);
		fprintf(stderr, "Raw File len=%d, will gen aes.c.enc\n", ret);
 
    	AES_init_ctx_iv(&ctx, key, iv); /*初始化加密 context, 指定key 和iv */
		//CBC 的openssl equivalent 命令，和 tiney AES 的 function
		//openssl aes-256-cbc -e -nosalt -v -nopad -K  1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef -iv 1234567890abcdef1234567890abcdef -in aes.c -out aes.c.openssl_enc
    	//AES_CBC_encrypt_buffer(&ctx, buf, ret);  
		//CTR 的 openssl equivalent 命令
		//openssl aes-256-ctr -e -nosalt -v -nopad -K  1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef -iv 1234567890abcdef1234567890abcdef -in aes.c -out aes.c.openssl_enc
		//加密後，結果會直接覆蓋原內容
		AES_CTR_xcrypt_buffer(&ctx, buf, ret);
		fp2=fopen("aes.c.enc", "wb"); //將加密後的資料，寫入另一個檔案
		for(i=0;i<ret;i++) {
			c=  buf[i];
			fputc(c, fp2);
		}
		fclose(fp2);
	}
	{ //Do decryption 
		struct AES_ctx ctx; //將加密後的檔案全部讀進 buffer
		fp=fopen("aes.c.enc", "r");
		ret = fread(buf, 1, sizeof(buf), fp);
		fclose(fp);
		fprintf(stderr, "Encrypted len=%d, will gen aes.c.dec\n", ret);
 
		//初始化 context, 指定 key 和 iv
		AES_init_ctx_iv(&ctx, key, iv);
		//AES_CBC_decrypt_buffer(&ctx, buf, ret);
		//AES_CTR_xcrypt_buffer(&ctx, buf, ret);
		//此處為了模擬掉資料，我們以loop一次解 16byte. 
		//即使是掉包，也要隨便丟個東西進去解。掉的那個 block 資料會錯
		//但下個block就會正確了.
		for(i=0;i<ret/16;i++) {
			if(i == -1) { 
				fprintf(stderr, "Insert a packet loss simulation in this test.\n");
				//simulate packet loss. kick the counter
				AES_CTR_xcrypt_buffer(&ctx, empty, 16);
				continue;
			}	
			//一次解密一個 block (16bytes)
			AES_CTR_xcrypt_buffer(&ctx, buf+i*16, 16);
		}
		fp2=fopen("aes.c.dec", "wb");
		for(i=0;i<ret;i++) {
			c=  buf[i];
			fputc(c, fp2);
		}
		fclose(fp2);
	}//
	
	return 0;
 
}

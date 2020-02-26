/* fseek example */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
unsigned char rsaKey[2000] = {};
unsigned char *rsa_ptr = NULL;
int main ()
{
  int rsaKeyFd;
  int rsaKeyLen = 0;
  rsaKeyFd = open("/home/disk_D/henry03/cTest/FILE_Class/API_open/rsa", O_RDONLY);
  if (rsaKeyFd >= 0) {
//                lseek(rsaKeyFd, 26, SEEK_CUR);
		rsaKeyLen = read(rsaKeyFd, rsaKey, sizeof(rsaKey) - 1);
                printf("[0] = 0x%2x , [1] = 0x%2x , [26] = 0x%2x , [27] = 0x%2x\n",rsaKey[0],rsaKey[1],rsaKey[26],rsaKey[27]);
                rsa_ptr = rsaKey +26;




		close(rsaKeyFd);
	}
  else{
	printf("not found\n");
  }
  int i=0;
  printf("%d %d\n",strlen(rsaKey),rsaKeyLen);
  for(i=0;i<rsaKeyLen;i++){
	if(i!=0 && (i%16)==0){
		printf("\n%02x ",rsaKey[i]);
        }else{
		printf("%02x ",rsaKey[i]);
  	}
  }
printf("\n");

  printf("\n");
  for(i=0;i<rsaKeyLen-26;i++){
        if(i!=0 && (i%16)==0){
                printf("\n%02x ",rsa_ptr[i]);
        }else{
                printf("%02x ",rsa_ptr[i]);
        }
  }
  printf("\n");




  return 0;
}

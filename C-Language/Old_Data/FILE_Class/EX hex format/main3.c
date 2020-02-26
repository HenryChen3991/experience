#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void){
FILE *sf;  
sf = fopen("bcm4360_map.bin", "r+");  
int n;  
unsigned char buf[16];  
unsigned long count = 0;  
printf("len = %ld\n",ftell(sf));
fseek( sf,  218, SEEK_SET );
printf("len = %ld\n",ftell(sf));
//fseek( sf,  ftell(sf), SEEK_SET );
char sc= 0xfe;
unsigned short ss = 0x1a13;
char byteblock[2];
byteblock[0] = (ss & 0xff);
byteblock[1] = (ss >> 8) & 0xff;
printf("0 : 0x%2x\n",byteblock[0]);
printf("1 : 0x%2x\n",byteblock[1]);
/*
memset(byteblock,0,sizeof(byteblock));
printf("0 : 0x%2x\n",byteblock[0]);
printf("1 : 0x%2x\n",byteblock[1]);
*/
fprintf(sf,"%c",byteblock[0]);
fseek( sf,  219, SEEK_SET );
fprintf(sf,"%c",byteblock[1]);
fseek( sf, 218, SEEK_SET );
while((n = fread(buf, sizeof(char), 16, sf)) > 0) {  
    int i;  
  
    printf("%08lx ", count);  
  
    for(i=0; i<n; i++) {  
        printf("%02x ", (unsigned)buf[i]);  
    }  
  
    if(n < 16) {  
        for(i=n; i<16; i++) printf("    ");  
    }  
  
    for(i=0; i<n; i++) {  
        putchar(isprint(buf[i]) ? buf[i] : '.');  
    }  
  
    putchar('\n');  
  
    count += 16;  
  
}  
fclose(sf);  


}

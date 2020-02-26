#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void){
FILE *sf;  
sf = fopen("bcm4360_map.bin", "rb");  
int n;  
unsigned char buf[16];  
unsigned long count = 0;  
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

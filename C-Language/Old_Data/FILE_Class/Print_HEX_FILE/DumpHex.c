#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void help(void)
{
	printf("usage:\n");
	printf("./PrintHex <file>\n");
}

void main(int argc,char *argv[]){
	if(argc!=2){
		help();
		exit(1);
	}

	FILE *sf;  
	sf = fopen(argv[1], "rb");
	if( sf == NULL ){
		printf("File %s could not be read\n",argv[1]);
		exit(1);
	}
	int n;  
	unsigned char buf[16];  
	unsigned long count = 0;  
	while((n = fread(buf, sizeof(char), 16, sf)) > 0) {  
    		int i;  
  
    		printf("%08lx ", count);  
  
    		for(i=0; i<n; i++) {  
        		printf("%02x ", (unsigned)buf[i]);  
    		}  
  
    		if(n < 16) {
        		for(i=n; i<16; i++) printf("   ");  
    		}  

		for(i=0; i<n; i++) {  
        		putchar(isprint(buf[i]) ? buf[i] : '.');  
    		}  
  
    		putchar('\n');  
  
 		count += 16;  
	}  
	fclose(sf);  
}

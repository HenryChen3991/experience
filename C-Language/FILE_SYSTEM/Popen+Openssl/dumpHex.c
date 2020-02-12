#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "debug.h"

void dump_hex_api(unsigned char *buf,int len)
{
	int n = 0;
	unsigned long count = 0;

	while((count < len)) {
    		int i;

    		printf("%08lx ", count);

		if( ((len-count)/16) >= 1 )
			n = 16;
		else
			n = (len-count)%16;

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
		buf += 16;
	}

}

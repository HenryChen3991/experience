#include <stdio.h>
#include <stdlib.h>
int parseHwVer(unsigned char *str)
{
        printf("str = %c (%d)\n",*str,*str);
        if( str[0] >= '0' && str[0] <= '9' )
		return 1;
	else 
		return 0;
}
//C/C++ function to convert an ANSI hexadecimal string to an int
//Keywords: Convert hexadecimal string int C/C++
int xtoi(char *hexstring)
{
	int	i = 0;
	
	if ((*hexstring == '0') && (*(hexstring+1) == 'x'))
		  hexstring += 2;
	while (*hexstring)
	{
		char c = toupper(*hexstring++);
		if ((c < '0') || (c > 'F') || ((c > '9') && (c < 'A')))
			break;
		c -= '0';
		if (c > 9)
			c -= 7;
		i = (i << 4) + c;
	}
	return i;
}


void main(void)
{
    unsigned char * c = "a0";
    unsigned char d = 0xfe;
    printf("%d\n",parseHwVer(c));
    printf("d = %d\n",d);

char buf[80];
    int i;

    scanf("%s",buf);
    i = xtoi(buf);
    printf("Read %s: converted to %d\n", buf, i);

    unsigned char aa[1];
    aa[0] = 0xf1;
    printf("%x\n",aa[0]);
    unsigned char bb[1];
    sprintf(bb,"%s",aa);
    printf("bb = %x\n",bb[0]);

/*
    c = 1;
    d = 1;
    printf("c = %x\n",c);
    
    if( c == 0 )
	printf("c == 0\n");
    else
	printf("c != 0\n");


    if( c == d)
	printf("c == d\n");
    else
	printf("c != d\n");
*/
}

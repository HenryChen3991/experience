#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//trim head and tail
char * strtrim(char *s) {
    char *p = s;
    char *q = s;
    while (*p==' ' || *p=='\t') 
        ++p;
    while ((p != NULL)&&(q != NULL)&&(*p != '\0')&&(*p != '\0'))
    {
        *q++ = *p++;
    };
    q -= 2;
    while (*q==' ' || *q=='\t') 
        --q;
    *(q+1) ='\0';
    return s;
}
//trim all of space
char * strtrimc( char * s )
{
    char * p1 = s;
	char * p2 = s;
	while(*p1 != '\0')
	{
		while(*p1 == ' ' || *p1 == '\t')
		{
			p1 ++;
		}
		* p2 ++ = *p1 ++;
	}
	*p2 = '\0';
	return (s);
}
void main(void)
{
	char str[1000] = {" adf asdfasldf jjjjjjjjj    iiii "};
	char str1[1000] = {" adf asdfasldf jjjjjjjjj    iiii "};
	puts("Start main.");
    printf("%s\n",strtrimc(str));
	printf("%s\n",strtrim(str1));
}

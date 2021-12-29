#include <stdio.h>
#include "gtk_string.h"
char * strtrim(char *s) {
    char *p = s;
    char *q = s;

    //去掉行首的空格
    while (*p==' ' || *p=='\t') 
        ++p;
    //赋值
    while ((p != NULL)&&(q != NULL)&&(*p != '\0')&&(*p != '\0'))
    {
        *q++ = *p++;
    };

	//删除'\0'字符 注意是 -2 上面q++是先操作再自加
    q -= 2;
    //去掉行末的空格
    while (*q==' ' || *q=='\t') 
        --q;
    //给字符串添加字符结束标志
    *(q+1) ='\0';
    //这里的return s要注意看好
    //因为p q经过一系列操作后，已经不是原来的位置，越界了 ，s还在原来位置，所以return s才是正确的。
    return s;
}

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "gtk_string.h"
#include "ckl_debug.h"

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


void cmsUtl_strToUpper(char *string)
{
   char *ptr = string;
   for (ptr = string; *ptr; ptr++)
   {
       *ptr = toupper(*ptr);
   }
}

void cmsUtl_strToLower(char *string)
{
   char *ptr = string;
   for (ptr = string; *ptr; ptr++)
   {
       *ptr = tolower(*ptr);
   }
}

int cmsUtl_strcmp(const char *s1, const char *s2) 
{
   char emptyStr = '\0';
   char *str1 = (char *) s1;
   char *str2 = (char *) s2;

   if (str1 == NULL)
   {
      str1 = &emptyStr;
   }
   if (str2 == NULL)
   {
      str2 = &emptyStr;
   }

   return strcmp(str1, str2);
}

void print_bin(int n)
{
    int l = sizeof(n)*8;//總位數。
    int i;
    printf("%s",LIGHT_CYAN);
    if(i == 0)
    {
         printf("0");
         return;
     }
    for(i = l-1; i >= 0; i --)//略去高位0.
    {
        if(n&(1<<i)) break;
    }
 
    for(;i>=0; i --)
        printf("%d", (n&(1<<i)) != 0);
    printf("%s\n",NONECOLOR);
}


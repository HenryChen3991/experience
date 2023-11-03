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

void print_short_type_bin(int n)
{
    int c = 0, k = 0;
    printf("%s",LIGHT_CYAN);
    for (c = 15; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
          printf("1");
        else
          printf("0");
    }
    printf(" --- %s\n",NONECOLOR);
    int *ptr =NULL;
    *ptr=10;
}

void print_int_type_bin(int n)
{
    int l = sizeof(int)*8;//總位數。
    int c = 0, k = 0;
    printf("%s",LIGHT_CYAN);
    for (c = 31; c >= 0; c--)
    {
        k = n >> c;

        if (k & 1)
          printf("1");
        else
          printf("0");
    }

    printf(" --- %s\n",NONECOLOR);
}

char *cmsUtl_strncpy(char *dest, const char *src, int dlen)
{

   /* if the dest ptr is NULL, we cannot copy at all.  Return now */
   if (dest == NULL)
   {
      ERROR("dest is NULL!");
      return NULL;
   }

   /* if src ptr is NULL, copy an empty string to dest */
   if (src == NULL)
   {
      return strcpy(dest, "");
      return dest;
   }

   /* do a modified strncpy by making sure dest is NULL terminated */
   if( strlen(src)+1 > (unsigned int) dlen )
   {
      NOTICE("truncating:src string length > dest buffer");
      strncpy(dest,src,dlen-1);
      dest[dlen-1] ='\0';
   }
   else
   {
      strcpy(dest,src);
   }
   return dest;
}

int hex2int(char *hex) {
    int val = 0;
    while (*hex) {
        char byte = *hex++;
       if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}


#include <stdio.h>
int main()
{
 char buf[512] = {0};
//1. 常见用法。
 sscanf("123456 asdf", "%s", buf);
 printf("%s\n", buf);
 printf("123456 Expect! %%s\n\n");
//结果为：123456
//2. 取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。
 sscanf("123456 ", "%4s", buf);
 printf("%s\n", buf);
  printf("1234 Expect! %%4s\n\n");
//结果为：1234
//3. 取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。
 sscanf("123456 abcdedf", "%[^ ]", buf);
 printf("%s\n", buf);
 printf("123456 Expect! %%[^ ]\n\n");
//结果为：123456
//4.  取仅包含指定字符集的字符串。如在下例中，取仅包含1到9和小写字母的字符串。
 sscanf("123456abcdedfBCDEFxyz", "%[1-9a-z]", buf);
 printf("%s\n", buf);
 printf("123456abcdedf Expect! %%[1-9a-z]\n\n");
//结果为：123456abcdedf
//5.  取到指定字符集为止的字符串。如在下例中，取遇到大写字母为止的字符串。
 sscanf("123456 abcdedfBCDEFxyz", "%[^A-Z]", buf);
 printf("%s\n", buf);
  printf("123456 abcdedf Expect! %%[^A-Z]\n\n");
//结果为：123456 abcdedf
//6、给定一个字符串iios/12DDWDFF@122，获取 / 和 @ 之间的字符串，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中
 sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]", buf);
 printf("%s\n", buf);
 printf("12DDWDFF Expect! %%*[^/]/%%[^@]\n\n");
//结果为：12DDWDFF
//7、给定一个字符串““hello, world”，仅保留world。（注意：“，”之后有一空格）
 sscanf("hello, world",  "%*s%s",  buf);
 printf("%s\n", buf);
 printf("world Expect! %%*s%%s\n\n");
//结果为：world 
//8、参数size的控制
 unsigned char m[6] = {0};
 int n = 0;
// sscanf 返回输入了几个参数
 n = sscanf("010203040506", "%hhx%hhx%hhx%hhx%hhx%hhx", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]);
 printf("hhx: %d :%02x%02x%02x%02x%02x%02x\n",n, m[0], m[1],m[2],m[3], m[4], m[5]);
// 结果 n 为 1， 因为 hhx 把所有字符串作为一个数字，后面的都没有输入
        
 n = sscanf("010203040506", "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]);
 printf("2hhx: %d :%02x%02x%02x%02x%02x%02x\n",n, m[0], m[1],m[2],m[3], m[4], m[5]);
// 结果 n 为 6， 加了2，之后限制每次hhx只匹配2个字符，所以每个都被输入
 n = sscanf("010203040506", "%02x%02x%02x%02x%02x%02x", &m[0], &m[1], &m[2], &m[3], &m[4], &m[5]);
 printf(" 02x: %d :%02x%02x%02x%02x%02x%02x\n",n, m[0], m[1],m[2],m[3], m[4], m[5]);
// 0 是填充字符，2表示匹配2个字符，x表示一个十六进制数
// 注意：编译有warning，有时结果是正确的（跟具体的编译器、编译选项、编译环境有关），但x表示4个字节，大于被输入对象的大小
// 这会导致相邻的3个字节会被覆盖！！这会导致分配在相邻内存的变量被清0
//http://blog.csdn.net/rainharder/archive/2008/09/27/2989675.aspx里面有个覆盖的例子
 return 0;
}

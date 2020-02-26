#include <stdio.h>
void main()
{
    FILE * fp;
    char buffer[80];
    char * str=NULL;

    fp = popen("cat a.c", "r");
    fgets(buffer, sizeof(buffer), fp);
    printf("%s", buffer);
    str = buffer;
    printf("str = %s\n",str);
    pclose(fp);
}

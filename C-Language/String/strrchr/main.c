#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *parserFileName(char *fileName)
{
	char *lastSlash = NULL;
	if(fileName == NULL)
		return "NULL";

	lastSlash = strrchr(fileName, '/');
	return (lastSlash != NULL) ? lastSlash + 1 : fileName;
}
void main(void){
    char str[128]="../../src/me/omci_dot1ag_maintenance_association.c";
    printf("%s\n",parserFileName(str));
    char str1[128]="omci_dot1ag_maintenance_association.c";
    printf("%s\n",parserFileName(str1));
    char str2[128]="omci.c";
    printf("%s\n",parserFileName(str2));
}

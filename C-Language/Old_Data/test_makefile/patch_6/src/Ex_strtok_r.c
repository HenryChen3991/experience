#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
 char pSrc[] = "this/is///jason;///i/am/from/xi'an/china///ssff";
 char* pToken = NULL;
 char* pSave = NULL;
 char* pDelimiter = "/";
 pToken = strtok_r(pSrc, pDelimiter, &pSave);
 printf("Begin:\n");
 while(pToken){
  printf("   pToken[%s]; pSave[%s]\n",pToken,pSave);
  pToken = strtok_r(NULL, pDelimiter, &pSave);
 }
 printf("End!\n");
 return 0;
}

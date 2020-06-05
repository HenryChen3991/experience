int macNumToStr(unsigned char *macAddr, char *str)
{
   if (macAddr == NULL || str == NULL)
       return 0;

   sprintf(str, "%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
           macAddr[0], macAddr[1], macAddr[2],
           macAddr[3], macAddr[4], macAddr[5]);
   return 1;
}
int hashToPwdStr(unsigned char *h,char *buffer)
{
   if (h == NULL || buffer == NULL)
       return 0;
   sprintf(buffer, "%2.2X%2.2X%2.2X%2.2X%2.2X",
           h[1], h[3], h[5],
           h[28], h[30]);
   return 1;
}


NVRAM_DATA *nvramData = NULL;
char mac_str[12]={0};
unsigned char h256[32]={0};
unsigned char pwd[10]={0};
macNumToStr(nvramData->ucaBaseMacAddr, mac_str);
sha256((const unsigned char *)mac_str, strlen(mac_str), h256);
hashToPwdStr(h256,(char *)pwd);

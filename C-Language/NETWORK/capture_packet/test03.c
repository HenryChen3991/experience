#include<stdio.h>
#include<pcap.h>

int main(int argc,char *argv[])
{
    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces,*temp;
    int i=0;
    if(pcap_findalldevs(&interfaces,error)==-1)
    {
        printf("\nerror in pcap findall devs");
        return -1;   
    }

    printf("\n the interfaces present on the system are:");
    for(temp=interfaces;temp;temp=temp->next)
    {
        printf("\n%d  :  %s",i++,temp->name);
       
    }
   
    return 0;


}

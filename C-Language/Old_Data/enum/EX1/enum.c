#include<stdio.h>
#include<stdlib.h>

typedef enum{
	Null,
	Jan,
	Feb,
	Mar,
	Apr,
	May
}Month;

enum
{
   OuiOamStart,
   OuiTeknovus = OuiOamStart,
   OuiCtc,
   OuiNtt,
   OuiDasan,
   OuiDpoe,
   OuiKt,
   OuiPmc,
   OuiCuc,
   OuiOamEnd,
   Oui802Dot1 = OuiOamEnd,
   OuiKnownCount
};


int aa = Jan;
int main(int argc,char **argv)
{

	printf("OuiDpoe = %d\n",OuiDpoe);

	printf("aa = %d\n",aa);
	int mon=atoi( argv[1] );
	printf("mon = %d\n",mon);
	switch(mon){
	  case Jan:
          printf("一月\n"); break;
      case Feb:
          printf("二月\n"); break;
      case Mar:
          printf("三月\n"); break;
      case Apr:
          printf("四月\n"); break;
      case May:
          printf("五月\n"); break;
      default:
          printf("要輸入1到5之間的數喔\n");
    }
	printf("#######################\n");
	mon = Feb;
	printf("mon = %d\n",mon);
    return 0;       
}

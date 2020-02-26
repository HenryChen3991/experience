#include<stdio.h>
#include<stdlib.h>
struct std{
	char str;
	int h;
        short age;
};
struct number {
  int number1;
  int number2;
  int number3;
  char c;
  char *str[5];
  struct std std1;
};


void ppp(struct number *numStruct,char *str){

	printf("s = %s\n",str);

   #define Y(attr) numStruct->attr
  //printf("%s\n",Y(str));
	
}

int main(void)
{
    struct number numStruct;
     
    numStruct.number1 = 0;
    numStruct.number2 = 1;
    numStruct.number3 = 2;
    numStruct.c = 'c';

    printf("std size = %d\n",sizeof(struct std));
    

    #define X(name,parameter) \
        printf("%s %d\n",#name,sizeof( name.parameter  ) );
    printf("Single Test\n");
    X(numStruct,number1);
    X(numStruct,number2);
    X(numStruct,number3);
    X(numStruct,c);
    X(numStruct,std1);
    char *str[5] = { "number1","number2","number3","c","std1" };
    int i;
    printf("Forloop Test size %d\n", sizeof(*str));
    for(i=0; i<5; i++)
    {
        printf("Test Str %s \n", str[i]);
        X(numStruct, str[i]);
    }

   /*#define Z(array,i) (char *)array[i]
	printf("%s\n",Z(str,0));

    ppp(&numStruct,(char *)str[0]);*/


    return 0;
}

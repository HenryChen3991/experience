#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
    FILE *fp = fopen("fscanf.txt","r");
    int aa[6]={-1,-1,-1,-1,-1,-1};
    int i = 0;
    while (!feof(fp))
    {
     fscanf(fp,"%d,%d,%d\n",aa+i,aa+i+1,aa+i+2);
     i = i + 3;
    }
    int j = 0;
    for (j=0; j<6; j++)
     printf("aa[%d] = %d\n",j,aa[j]);

    return 0;
}

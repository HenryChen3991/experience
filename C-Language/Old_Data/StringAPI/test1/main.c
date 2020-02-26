#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ON 1
#define OFF 0
void main(void)
{
  FILE *fp = fopen("./data","rw+");
  if(fp)
  {
     char state[12];
     fgets(state,sizeof(state),fp);
	printf("len = %d\n",strlen(state));
     if (state[strlen(state) - 1] == '\n')
	state[strlen(state) - 1] = '\0';
     printf("state =%s\n",state);
     char buf;
	int a;
 	scanf("%d",&a);
	sprintf(&buf, "%d", a);
printf("buf = %c\n",buf);
	fseek( fp, 0, SEEK_SET );
	 fwrite(&buf, 1,sizeof(buf),fp);
     fclose(fp);
     

     fp = fopen("./data","r");
     int  c;
//     do{
     	c = fgetc(fp);
	if( feof(fp) ){
		printf("had bad symbol\n");
//		break;
	}else{
     		printf("%c (%x)  (%d)\n",c,c,c);
	}
  //   }while(1);

     fclose(fp);


	if( c == ON )
		printf("equal\n");

  }

}

#include <stdlib.h>
#include <stdio.h>
char *fInName, *fOutName;
FILE *fIn, *fOut;


int subTid(char *ptr) {
        if(*(ptr+7)==')') fprintf(fOut, "00 0%c ", *(ptr+6));
   else if(*(ptr+8)==')') fprintf(fOut, "00 %c%c ", *(ptr+6), *(ptr+7));
   else if(*(ptr+9)==')') fprintf(fOut, "0%c %c%c ", *(ptr+6), *(ptr+7), *(ptr+8));
   else fprintf(fOut, "%c%c %c%c ", *(ptr+6), *(ptr+7), *(ptr+8), *(ptr+9));
}
int subType(char *ptr) {
   if( *(ptr+5) == '4' ||  *(ptr+5) == '8' ||  *(ptr+5) == '9')
     fprintf(fOut, "4%c ", *(ptr+5));
   else if( *(ptr+5) == '2' && *(ptr+6) == '6' )
     fprintf(fOut, "1A ");     
   fprintf(fOut, "0a ");
}
int subClassId(char *ptr) {
   char *str_at = strchr(ptr,'@');
   char *str_comma_s = strstr(ptr+2,"(0x");
   char *str_comma_e = strchr(ptr+2,')');
   char str_class[20]={'\0'};
   char str_entity[20]={'\0'};
   strncpy(str_class,(ptr+2),str_at-ptr-2);
//   printf("str_class = %s\n", str_class);
   strncpy(str_entity, str_comma_s+3, str_comma_e-str_comma_s-3);
//   printf("str_entity = %s\n", str_entity);

   unsigned int int_class = atoi(str_class);
   if(int_class <= 255)
     fprintf(fOut, "00 %0.2x ", int_class);
   else
     fprintf(fOut, "%0.2x %0.2x ", int_class/256, int_class%256);
   if(strlen(str_entity) == 4) fprintf(fOut, "%c%c %c%c ", str_entity[0], str_entity[1], str_entity[2], str_entity[3]);
   if(strlen(str_entity) == 3) fprintf(fOut, "0%c %c%c ", str_entity[0], str_entity[1], str_entity[2]);
   if(strlen(str_entity) == 2) fprintf(fOut, "00 %c%c ", str_entity[0], str_entity[1]);
   if(strlen(str_entity) == 1) fprintf(fOut, "00 0%c ", str_entity[0]);
}
int subContent(char *ptr) {
   char *str_end = strchr(ptr,'|');
   char *str_content[100]={'\0'};
   strncpy(str_content, ptr, str_end-ptr);
   fprintf(fOut, "%s $\n", str_content);
}

int sub1() {
   char str[500];
   char strDS[] = "->";
   char *ptr;
   int DS = 0;
   while( fgets (str, 200, fIn)!=NULL ) {
      if (strstr(str, strDS) != NULL) {
//        printf("%d %s\n", DS, str);
        ptr = strstr(str,"Tid(0x");
        if(ptr!=NULL) subTid(ptr);
        ptr = strstr(str,"Type(");
        if(ptr!=NULL) subType(ptr);
        ptr = strstr(str,"Tid(0x");
        ptr = strstr(ptr,")|");
        if(ptr!=NULL) subClassId(ptr);
        DS = 1;
      }
      else if(DS == 1) {
        subContent(str+1);
//        printf("%s\n", str+1);
        DS = 0;
      }
      else
        printf("");
   }
}

int main(int argc, char *argv[]) {
  fInName = argv[1];
  fIn = fopen(fInName, "r");
  if(fIn!=NULL) {
    printf("fIn = %s\n", fInName);
    fOutName = strcat(fInName, "_MB.txt");
    fOut = fopen(fOutName, "w");
    if(fOut!=NULL) {
      printf("fOut  = %s\n", fOutName);
      sub1();
      fclose(fOut);
    }
    fclose(fIn);
  }
  return 0;
}

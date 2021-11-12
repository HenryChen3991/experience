#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int a;
	int b;
	char c;
	char d[10];
} gtk_st;

typedef struct{
	char a[50];
	char b[50];
	int c;
} gtk_bt;

void main(void)
{
	char buf[sizeof(gtk_st)+sizeof(gtk_bt)]={0};
	gtk_st *st = (gtk_st *)buf;
	gtk_bt *info = (gtk_bt *)(st+1);

	printf("buf size = %d, gtk_st size = %d, gtk_bt size =%d\n",sizeof(buf),sizeof(gtk_st),sizeof(gtk_bt));
	printf("gtk_st addr = %p, gtk_bt addr = %p\n",st,info);
	
}

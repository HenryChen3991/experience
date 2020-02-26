#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void usage(){
	printf("Usage():\n");
	printf("\tm\t--create a file\n");
	printf("\tr\t--use remove to remove a file\n");
}
void main(int argv,char * argc[])
{
	if(argv==2){
		if(!strcmp(argc[1],"m")){
			system("echo aaa > a.c");
		}
		else if(!strcmp(argc[1],"r")){
			printf("before use function remove()\n");
			system("ls");
			remove("a.c");
			printf("after use function remove()\n");
			system("ls");
		}
		else if(!strcmp(argc[1],"?")){
			usage();
		}
		else{
			usage();
		}
	}
}


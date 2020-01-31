#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEBUG_FLAG 0
#define DEBUG(msg,arg...) printf("%s:%s:%d " msg"\n",__FILE__,__FUNCTION__,__LINE__,##arg)

void analyze_parameter(int argc,char *argv[],char **file)
{
	char *action=NULL;

#if 0
/*Method 1*/
        //So, as others also explained you have to know the length of the string you want to allocate
        //, and then add 1 to store the terminal \0, C implicitly use to put at the end of strings.
        *file = (char*)malloc(strlen(argv[1])+1);
        snprintf(*file,strlen(argv[1])+1,"%s",argv[1]);
#endif //0
	
/*Method 2*/
        //stdrup可以直接把要復制的內容復制給沒有初始化的指針，因為它會自動分配空間給目的指針
        //最後必須釋放內存
        *file = strdup(argv[1]);
        action = argv[2];

	DEBUG("file = %s, action = %s",*file,action);
}
void main(int argc,char *argv[])
{
        char *file=NULL;
	analyze_parameter(argc,argv,&file);
	DEBUG("file = %s",file);
	free(file);
}

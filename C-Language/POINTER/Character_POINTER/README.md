一、Character Pointer
==========================================
* 情境
	* 傳遞char *參數並且達到call by reference的效果，下面的範例中可以看出當執行完function "analyze_parameter"後，<font color="red">變數file_name並沒有達到預期的效果(結果為NULL)</font>
		```c
    	void analyze_parameter(char *file_name)
    	{
    		file_name = argv[1];
        	printf("file_name = %s\n",file_name); //file_name = certificatehost
    	}
    	void main(int argc,char *argv[])
    	{
    		char *file_name = NULL;
        	analyze_parameter(file_name);
        	printf("file_name = %s\n",file_name); //file_name = null
    	}
* 解決方式: 
	* 必須傳遞變數file_name的位址，故需利用一個char ** ptr來接收
		```c
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

* strdup函數分析
	* 函數原型：
		```c
		#include <string.h>
		char *strdup(const char *s); 
    * 功能
   		* strdup()函數主要是拷貝字符串s的一個副本，由函數返回值返回，這個副本有自己的內存空間，和s沒有關聯。
        * <font color="red"> strdup函數復制一個字符串，使用完後，要使用delete函數刪除在函數中動態申請的內存</font>
   		* strdup函數的參數不能為NULL，一旦為NULL，就會報段錯誤，因為該函數包括了strlen函數，而該函數參數不能是NULL。
    * strdup source code
   		```c
        # define __strdup strdup
        char * __strdup (const char *s)
		{
  			size_t len = strlen (s) + 1;
  			void *new = malloc (len);
  			if (new == NULL)
    			return NULL;
  			return (char *) memcpy (new, s, len);
		}
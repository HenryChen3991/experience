一、Function Pointer
==========================================
* 即為<font color="red">儲存某一個函式起始memory address的變數</font>，此變數可以提供我們在之後進行呼叫。

	```c
    1.現在假設我們有以下這個函式：
	  int Square(int n)  
	  {  
	      return n*n;  
	  }
    2.然後宣告一個function pointer變數，以便於指向函式Square
      int (*fptr)(int);
	  fptr = Square; 
*	延伸:
	* <font color="red">Typedef</font> 
		- 是c/c++中的關鍵字，其允許programmer為data type(資料型態)創造一個全新的名字。同時也可以為函式創造一個別名
		```c
    	typedef int (*MathMethod)(int, int);  // 注意*要在()之中
 	
    	int Mul(int a, int b){
    		return a*b;
		}
	
    	int *FuncA(int x, int y, MathMethod Opt)
		{
    	 	return Opt(x, y);
		} 
 
		int result = FuncA(5, 10, Mul);
	* <font color="red">Initialization</font>
		```c
        //宣告時順便初始話指向的函式
        int (*MathMethod)(int, int) = NULL;
        int (*MathMethod)(int, int) = Mul;
	* <font color="red">Array of Function Pointer (指標函式陣列) </font> 
		* 所謂的array of function pointer是一個儲存function pointer的array
		```c
        #include <stdio.h>
  		#include <string.h>
  		#include <stdlib.h>
        
        typedef int (*MathMethod)(int, int); 
		//typedef int (*MathMethod())(int, int);

		int Mul(int a, int b) { return a*b; }
		int Divide(int a, int b) { return a/b; }
		int Minus(int a, int b) { return a-b; }
		int Add(int a, int b) { return a+b; } 

		int (*calcArray[])(int, int) =   // Array of function pointer，存放會使用到的function名稱
		{ 
    		Mul,     // 這邊亦可以打&Mul，&可省略
    		Divide, 
    		Minus, 
    		Add 
		};
		char* operation[] =   //做為判斷的自訂條件，與argv比對用
		{
    		"x",
    		"/",
    		"-",
    		"+"
		};

		int main(int argc, char** argv)
		{
    		int size = sizeof(calcArray) / (sizeof(int(*)));    // 使用此方法可以避免每次增加功能時修改長度
    		int i = 0;

    		if(argc > 1)
        		for(i; i<size; i++)
        		{
            		if( strcmp(argv[2], operation[i]) == 0)
            		{
               		 	int a = atoi(argv[1]), b = atoi(argv[3]);
                		printf("%d %s %d = %d\n", a, argv[2], b, calcArray[i](a, b));
                		break;
            		}
        		}
		}
 
        執行結果:
		./bin/Array_function_pointer 1 + 2
		1 + 2 = 3
		./bin/Array_function_pointer 1 - 2
		1 - 2 = -1
		./bin/Array_function_pointer 1 x 2
		1 x 2 = 2
		./bin/Array_function_pointer 1 / 2
		1 / 2 = 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//typedef int (*MathMethod())(int, int); 
typedef int (*MathMethod)(int, int); 

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

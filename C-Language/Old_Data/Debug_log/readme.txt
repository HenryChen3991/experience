compile:
	gcc debug_log.c main.c -o main -DDEBUG=0

	gcc debug_log.c main.c -o main -DDEBUG=1

	gcc debug_log.c main.c -o main -DDEBUG=2

	gcc debug_log.c main.c -o main -DDEBUG=3

------------------------------------------------------------------------------
軟體中的黑盒子 (介紹 va_lsit, va_start, va_end... 等)

這篇文章主要介紹的 macro 及 method 有

va_list

va_arg

va_start

va_end

##

__FUNCTION__

__VA_ARGS__ 

當我們在開發任何軟體或小程式時，記錄檔(log)扮演著一個非常重要的角色。因為當我們的軟體無法正常工作或使用異常時，電腦的作業系統並不一定會告訴使用者目前所使用的小程式或軟體出現了什麼樣的問題。

記錄檔(log)就像是飛航器中的黑盒子一樣，可以讓程式開發人員記錄程式在運行中發生了什麼樣的問題或遇到了什麼樣的狀況，因此記錄檔內容的品質好壞及容不容易使用就變成了非常重要的關鍵點。

首先先來記錄一下，我在C++中使用 va_list, va_start, va_end ... 等相關 macro 及 API 來記錄程式運作的經驗。

通常在寫 log 時，我都會寫下發生的問題以及引起這個問題的 method  名稱。但是問題來了，如果在每個地方都要寫下這些資訊，我在寫 log 就會變得繁瑣，整個程式碼看起來就會很亂，所以這時候我就會利用一些macro 及 method 來完成。

va_list
va_list 我猜應該是 variable list 的簡稱，從名字來看就可以知道是放一連串的 variable.

va_arg, va_start, va_end

va_arg 主要就是用來取得目前指向的變數，並且在指向下一個變數。例如:

   1: va_list ap; // 宣告可變長度變數
   2: va_start(ap, before); // 開始使用並讓 ap 指向 before
   3: auto variable = va_arg(ap, before); // 取得目前指向的變數並且指向下一個變數
   4: va_end(ap); // 告知結束訊息
 
## 和 __FUNCTION__
在使用 macro 來搭配不定個數參數使用時，如果是以下情形可能就會造成多一個 , 產生

1: #define ABC("%s", …) printf(format, __VA_ARGS__);

// 萬一沒有引數時會產生以下狀況 —> 多了一個逗號在後面。 printf("%s",);

所以這個時候就可以使用 ## 來提示 Compiler 是否有 ,

1: #define ABC("%s", ...) printf(format, ##__VA_ARGS__);

 

__FUNCTION__
這個 macro 就是目前 function 的名稱。例如:

   1: namespace pajace {
   2:     namespace utility {
   3:         void MyFunction(){
   4:             printf("%s ", __FUNCTION__);
   5:         }
   6:     }
   7: }
輸輸結果就會是: “pajace::utility::MyFunction”

 
程式碼範例:
最後，直接來看程式碼吧!!第一個部分是將傳入的字串一行一行列印出來。

 

   1: void printLine(char* first, ...){
   2:     char*str;
   3:     va_list ap;
   4:     
   5:     str = first;
   6:     va_start(ap, first);               // 開始使用 va_list
   7:     do {
   8:         printf("%s \n", str);
   9:         str = va_arg(ap, char*);    // 指向下一個參數
  10:     } while(str != NULL);
  11:     va_end(ap);                    // 停止使用
  12: }
  13:  
 

在使用 va_list 時一定要先呼叫 va_start() 並且將 va_list 及傳入的參數當作參數傳入給 va_start

   1: int main(){
   2:     printLine("first", "second", "third", "fourth", "fifth");
   3:     system("Pause");
   4:     return 0;
   5: }
   6:  
   7: 輸出結果:
   8: first
   9: second
  10: third
  11: fourth
  12: fifth
  13: 請按任意鍵繼續...

第二部分:傳入不固定長度的 int，找出最大值及列印數字。
   1: 1: void printInt(int amount, ...) {
   2:     int value;
   3:     va_list ap;
   4:     printf("Int passed:");
   5:  
   6:     va_start(ap, amount);
   7:     for(int i=0; i<amount; i++){
   8:         value = va_arg(ap, int);
   9:         printf("\t%d,", value);
  10:     }
  11:     va_end(ap);
  12: }
  13:  
  14: int findMax(int amount, ...){ //  因為無法得知目前共有多少個參數，所以拿第一個參數當作參數總個數來使用
  15:     int value = 0;
  16:     va_list ap;
  17:  
  18:     va_start(ap, amount); // 開始使用
  19:     int greater = va_arg(ap, int);  // 將第2個參數(也就是下一個參數) assign 給 greater
  20:     for(int i=1; i<amount; i++){   //  利用第一個參數 (amount)  當作參數總個數
  21:         value = va_arg(ap, int);
  22:         greater = (greater > value)?greater:value;
  23:     }
  24:     va_end(ap);  // 通知系統結束使用
  25:     return greater;
  26: }
  27:  
  28: int main(){
  29:     printInt(4, 1, 4, 7, 9);
  30:     int max = findMax(4, 111, 222, 777, 232);
  31:     printf("Max is %d\n", max);
  32:     system("Pause:);
  33:     return 0;
  34: }
  35:  
  36:  
  37: 輸出結果:
  38: Int passed: 1, 4, 7, 9
  39: Max is 777
  40: 請按任意鍵繼續...

第三部分:格式化輸出:

   1: void stringFromat(const char* format, ...){
   2:     va_list ap;
   3:     char* temp;
   4:     char line[64];
   5:  
   6:     memset(line, 0, sizeof(line));
   7:     strcpy_s(line, format);   
   8:  
   9:     va_start(ap, format);
  10:     while( (temp=va_arg(ap, char*)) != NULL)
  11:         strcat_s(line, temp);
  12:     va_end(ap);
  13:  
  14:     printf("%s\n", line);
  15: }
  16:  
  17: void stringFormatByvsprintf_s(const char* message, ...){
  18:     va_list ap;
  19:     char buffer[300];
  20:  
  21:     va_start(ap, format);
  22:     vsprintf_s(buffer, sizeof(buffer), message, ap);
  23:     va_end(ap);   
  24:  
  25:     printf("%s", buffer);
  26: }
-----------------------分隔線----------------------------

有了以上的觀念後，接下來就可以利用以上的 macro 來產生我們所需要的 log function 了!

   1: #define MyLog(x, ...)  logInfo("[MyApplication][%s] "x, __FUNCTION__, ##__VA_ARGS__);
   2: #define MyLog(x, ...)  logWarning("[MyApplication][$s][Warning] "x, __FUNCTION__, ##__VA_ARGS__);
   3: #define MyLog(x, ...)  logError("[MyApplication][$s][Error] "x, __FUNCTION__, ##__VA_ARGS__);
   4:  
   5:  
   6: void MyLog(char* message, ...){
   7:     va_list ap;
   8:     char buffer[128];
   9:  
  10:     va_start(ap, message);
  11:     vsprintf_s(buffer, sizeof(buffer), message, ap);
  12:     va_end(ap);    
  13:  
  14:     // 在這裡就可以將我們的 log 記錄下來，可以記錄在檔案中，也可以記錄在系統中讓 debug view 可以及時 catch 到
  15:     // 記錄到系統讓 debugView 可以抓到
  16:     OutputDebugString(buffer);
  17:  
  18:     // 寫入到檔案中
  19:     ...
  20:     ...
  21:     ...
  22:     
  23:     // 或做其他的事情
  24: }
  25:  
  26:  
  27: int main(){
  28:     // do something here
  29:     if(success){
  30:         logInfo("Success");
  31:     } else {
  32:         logError("這裡發生錯誤了!!! %d, %s", 100, "文字參數測試");
  33:     }
  34:  
  35:     return 0;
  36: }

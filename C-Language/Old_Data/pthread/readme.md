pthread_create 函數可以用來建立新的執行緒，並以函數指標指定子執行緒所要執行的函數，
子執行緒在建立之後，就會以平行的方式執行，
在子執行緒的執行期間，主執行緒還是可以正常執行自己的工作，
最後主執行緒再以 pthread_join 函數等待子執行緒執行結束，處理後續收尾的動作。


使用 gcc 編譯時，要加上 -lpthread 參數：
gcc hello.c -lpthread -o hello



資料傳遞
在許多的平行化應用程式中，我們都會需要傳遞一些資料給子執行緒進行計算，
而在計算完之後再將結果傳回來，而子執行緒在傳回資料時通常都會以 malloc 配置記憶體空間來存放傳回的資料，
以下是一個典型的範例：

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 子執行緒函數
void *child(void *arg) {
   int *input = (int *) arg; // 取得資料
   int *result = malloc(sizeof(int) * 1); // 配置記憶體
   result[0] = input[0] + input[1]; // 進行計算
   pthread_exit((void *) result); // 傳回結果
}

// 主程式
int main() {
   pthread_t t;
   void *ret; // 子執行緒傳回值
   int input[2] = {1, 2}; // 輸入的資料

   // 建立子執行緒，傳入 input 進行計算
   pthread_create(&t, NULL, child, (void*) input);

   // 等待子執行緒計算完畢
   pthread_join(t, &ret);

   // 取得計算結果
   int *result = (int *) ret;

   // 輸出計算結果
   printf("%d + %d = %d\n", input[0], input[1], result[0]);

   // 釋放記憶體
   free(result);

   return 0;
}


執行的輸出為：
1 + 2 = 3

這個程式中，子執行緒呼叫 malloc 配置了記憶體空間，而主執行緒在使用完該記憶體空間之後，負責釋放掉不再使用的記憶體。



********
pthread_detach

创建一个线程默认的状态是joinable, 如果一个线程结束运行但没有被join,则它的状态类似于进程中的Zombie Process,即还有一部分资源没有被回收（退出状态码），所以创建线程者应该调用pthread_join来等待线程运行结束，并可得到线程的退出代码，回收其资源（类似于wait,waitpid) 
但是调用pthread_join(pthread_id)后，如果该线程没有运行结束，调用者会被阻塞，在有些情况下我们并不希望如此，比如在Web服务器中当主线程为每个新来的链接创建一个子线程进行处理的时候，主线程并不希望因为调用pthread_join而阻塞（因为还要继续处理之后到来的链接），这时可以在子线程中加入代码 
pthread_detach(pthread_self()) 
或者父线程调用 
pthread_detach(thread_id)（非阻塞，可立即返回） 
这将该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源。


*********
pthread_join 

调用pthread_join的线程会阻塞，直到指定的线程返回，调用了pthread_exit，或者被取消。 
    如果线程简单的返回，那么rval_ptr被设置成线程的返回值，参见范例1；如果调用了pthread_exit，则可将一个无类型指针返回，在pthread_join中对其进行访问，参见范例2；如果线程被取消，rval_ptr被设置成PTHREAD_CANCELED。 
    如果我们不关心线程的返回值，那么我们可以把rval_ptr设置为NULL。 

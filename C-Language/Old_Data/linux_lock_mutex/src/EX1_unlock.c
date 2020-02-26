#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

const char filename[] = "hello_unlock";

void* thread(void *id){

        int num = *(int *)id;
	int i = 0;
        // 写文件的操作
        FILE *fp = fopen(filename, "a+");
        int start = *((int *)id);
        int end = start + 1;
        setbuf(fp, NULL);// 设置缓冲区的大小
        fprintf(stdout, "%d\n", start);
        for (i = (start * 10); i < (end * 10); i ++){
                fprintf(fp, "%d\t", i);
        }
        fprintf(fp, "\n");
        fclose(fp);

        return NULL;
}

int main(){
        int num_thread = 10;
	int i = 0;
        pthread_t *pt = (pthread_t *)malloc(sizeof(pthread_t) * num_thread);
        int * id = (int *)malloc(sizeof(int) * num_thread);

        for (i = 0; i < num_thread; i++){
                id[i] = i;
                if (pthread_create(&pt[i], NULL, thread, &id[i]) != 0){
                        printf("thread create failed!\n");
                        return 1;
                }
        }
        for (i = 0; i < num_thread; i++){
                pthread_join(pt[i], NULL);
        }

        // 释放资源
        free(pt);
        free(id);
        return 0;
}

#include "insertion-sort.h"

//Insertion Sort的方法為：
//			將第i張紙牌加入「前i−1張排序過」的紙牌組合，得到i張排序過的紙牌組合。

//#define ANIMATION (1) //
void insertion_sort(int *data,int size)
{
	int i = 0;
	int j = 0;

#ifdef ANIMATION
	for(i = 1 ; i < size ; i++){
		printf("Round %d :\n",i);
		int tmp = data[i];
		j = i-1;
		while( (tmp < data[j]) && j >= 0){
			printf("Data %d move to array %d position\n",data[j],j+1);
			data[j+1] = data[j];
			j--;
		}
		printf("Data %d move to array %d position\n",tmp,j+1);
		data[j+1] = tmp;
		print_data("After this round of sorting : ",data,size);
	}
#else
	for(i = 1 ; i < size ; i++){
		int tmp = data[i];
		j = i-1;
		while( (tmp < data[j]) && j >= 0){
			data[j+1] = data[j];
			j--;
		}
		data[j+1] = tmp;
	}
#endif //ANIMATION
}

void print_data(char *prefix,int *data,int size){
	NW_DEBUG_COLOR(LIGHT_GREEN,"%s",prefix);
	for(int i=0;i<size;i++)
		NI_DEBUG_COLOR(LIGHT_GREEN,"%d ",data[i]);
	printf("\n");
	SET_COLOR(NONECOLOR);
}

int main(void)
{
	DEBUG_COLOR(LIGHT_GREEN,"Welcome Insertion Sort Example");
	int data[6] = {1,30,20,70,100,2};
	int size = sizeof(data)/sizeof(int);
	print_data("Before sorting : ",data,size);
	insertion_sort(data,size);
	print_data("After  sorting : ",data,size);
	return 0;
}

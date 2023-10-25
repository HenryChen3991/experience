#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bubble-sort.h"

void swap(int *a,int *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
void bubble_sort(int *data,int size)
{
	int i = 0;
	int j = 0;
	for(i=0;i<size;i++){
		for(j=0;j<size-i-1;j++){
			if(data[j] > data[j+1])
			{
				swap(&data[j],&data[j+1]);
			}
		}
	}
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
	DEBUG_COLOR(LIGHT_GREEN,"Welcome Bubble Sort Example");
	int data[6] = {1,30,20,70,100,2};
	int size = sizeof(data)/sizeof(int);
	print_data("Before sorting : ",data,size);
	bubble_sort(data,size);
	print_data("After  sorting : ",data,size);
	return 0;
}

#include "quick-sort.h"

int main(void)
{
	return 0;
}


/*
Lomuto Partition Scheme
Lomuto Partition Scheme 是一種常見的 quick sort 實作方法。它所使用的 partition 函式的概念如下：

首先選出一個 pivot，這邊是用陣列內的最後一個元素 arr[hi]。
用 i 紀錄下一個小於等於 pivot 的元素所要放置的位置，初始化為 lo。
接著遍歷 arr，範圍從 lo 到 hi - 1。當發現小於等於 pivot 的元素時，就跟位於 i 的元素交換位置。每次交換完，就把 i 往前加一。
遍歷結束以後，再把位於 i 的元素和位於 hi 的元素 (也就是 pivot) 作交換。
最後回傳 i，它就是 pivot 的 index。
做完 partition， i 左邊的元素都會小於等於 pivot，右邊都會大於 pivot。實作如下：

function partition(arr, lo, hi) {
  // 選出 pivot
  const pivot = arr[hi];

  // 遍歷陣列，將小於等於 pivot 的元素和 i 位置的元素交換
  let i = lo;
  for (let j = lo; j < hi; ++j) {
    if (arr[j] <= pivot) {
      swap(arr, i, j);
      i++;
    }
  }

  // 將 pivot 和位於 i 的元素交換
  swap(arr, i, hi);
  return i;
}
基於這個 partition 的 quick sort 實作如下：

function quickSort(arr, lo, hi) {
  const p = partition(arr, lo, hi)
  quickSort(arr, lo, p - 1)
  quickSort(arr, p + 1, hi)
}

quickSort(arr, 0, arr.length - 1)
*/

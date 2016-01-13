#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

int cnt_sort(int* data, int size, int k)
{
    int* counts = NULL; /*计数数组*/
    int* temp = NULL;   /*保存排序后的数组*/

    if ((counts = (int *) malloc( k * sizeof(int))) == NULL)   /*申请数组空间*/
        return -1;
    if ((temp = (int *) malloc( k * sizeof(int))) == NULL)
        return -1;


    for (int i = 0; i < k; i ++) {  /*初始化计数数组*/
        counts[i] = 0;
    }

    for(int i = 0; i < size; i++){  /*数组中出现的元素，及出现次数记录*/
        counts[data[i]] += 1;
    }

    for (int i = 1; i < k; i++) {      // 对所有计数累加
        counts[i] += counts[i - 1];
    }

    for (int i = size -1; i >= 0; i --) // 反向遍历原来的数组
    {
        temp[counts[data[i]] - 1] = data[i]; // 在tmp中的位置
        counts[data[i]] -= 1; // 拉出一个元素 个数少1
    }

    memcpy(data,temp,size * sizeof(int));

    free(counts);
    free(temp);

    return 0;
}

int main()
{
    int a[] = {1,0,2,1,4,5,7,4};
    int size = sizeof(a) / sizeof(a[0]);
    int max = a[0];

    for(int i = 0; i < size; ++i){ // 1. find max in a[]
        if(a[i] > max){
            max = a[i];
        }
    }

    cnt_sort(a, size, max + 1); // 由于有0所以需要加1

    for(int i = 0; i < size; ++i){
        printf("%d ", a[i]);
    }

    return 0;
}

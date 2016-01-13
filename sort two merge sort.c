#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void printLn(int array[], int len)
{
    int i = 0;
    for(i = 0; i < len; ++i)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void Merge(int src[], int des[], int low, int mid, int high)
{
    int i = low;
    int j = mid + 1;
    int k = low;

    while( (i <= mid) && (j <= high) )
    {
        if(src[i] < src[j]){ // 将2路归并中 较小的先放入容器
            des[k++] = src[i++];
        }
        else{
            des[k++] = src[j++];
        }
    }
    while( i <= mid ) // 将剩余的部分放入容器
    {
        des[k++] = src[i++];
    }
    while( j <= high )
    {
        des[k++] = src[j++];
    }
}

void MSort(int src[], int des[], int low, int high, int max)
{
    if( low == high )
    {
        des[low] = src[low];
    }
    else
    {
        int mid = (low + high) / 2;
        int* space = (int*)malloc(sizeof(int) * max); /* 辅助空间 */
        if( space != NULL )
        {
            MSort(src, space, low, mid, max);         /* 左递归 */
            MSort(src, space, mid+1, high, max);      /* 右递归 */
            Merge(space, des, low, mid, high);        /* 归并拷贝 */
        }
        free(space);
    }
}

void MergeSort(int array[], int len) // O(n*logn)
{
    MSort(array, array, 0, len-1, len);
}
int main(void)
{
    int array[] = {21, 25, 49, 25, 16,8};
    int len = sizeof(array) / sizeof(*array);

    printLn(array, len);

    MergeSort(array, len);

    printLn(array, len);


    return 0;
}

#include <stdio.h>
#include <stdlib.h>

void printLn(int array[], int len)
{
    int i = 0;

    for(i = 0; i < len; ++i)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}
void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int partition(int array[], int low, int high)
{
    int rand_pos = rand() % ( high - low + 1 ) + low; // 随机位置作为基准
    swap(array, low, rand_pos);

    int pv = array[low]; // 选择了第一个元素作为基准 大的在后 小的在前

    while(low < high)
    {
        while((low < high) && (array[high] >= pv)){
            high--;
        }
        swap(array, low, high); // 小于pv跳出循环交换

        while((low < high) && (array[low]) <= pv){
            low++;
        }
        swap(array, low, high); // 大于pv跳出循环交换
    }
    return low;
}
void QSort(int array[], int low, int high)
{
    if(low <high)
    {
        int pivot = partition(array, low, high); // 分割

        QSort(array, low, pivot-1);
        QSort(array, pivot+1, high);
    }
}
void QuickSort(int array[], int len) // O(n*logn)
{
    QSort(array, 0, len-1);
}
int main(void)
{
    int array[] = {21, 25, 49, 25, 16,8};
    int len = sizeof(array) / sizeof(*array);

    printLn(array, len);

    QuickSort(array, len);

    printLn(array, len);


    return 0;
}

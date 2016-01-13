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

// 选择排序: 从头开始遍历 如果发现i之后的元素中有比它小的 则将i与最小的那个交换
void SelectionSort(int array[], int len)
{
    int i;
    int j;
    int min;

    for(i = 0; i < len; i++){
        min = i;
        for(j = i + 1; j < len; j++){
            if(array[j] < array[min]){ // 比标记的最小值还小
                min = j;
            }
        }

        if(min != i){
            swap(array, min, i);
        }
    }
}
int main(void)
{
    int array[] = {21, 25, 49, 25, 16,8};
    int len = sizeof(array) / sizeof(*array);

    printLn(array, len);

    SelectionSort(array, len);

    printLn(array, len);

    return 0;
}

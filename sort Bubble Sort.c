#include <stdio.h>
#include <stdlib.h>

void printLn(int array[], int len)
{
    int i = 0;

    for(i = 0; i < len; ++i)
    {
        printf("%d\n", array[i]);
    }

    printf("\n");
}

void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

/* 选择排序的思路是:将第i(第1个)与它后面的每个元素做对比 如果比它小则与它交换位置 */
void SelectionSort(int array[], int len)
{
    int i = 0;
    int j = 0;

    for(i = 0; i < len; i++)
    {
        for(j = i; j < len; j++)
        {
            if(array[j] < array[i]) 
            {
                swap(array, i, j);
            }
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
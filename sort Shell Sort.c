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

void ShellSort(int array[], int len) // O(nlogn)
{
    int i = 0;
    int j = 0;
    int k = -1;
    int temp = -1;
    int gap = len;

    do{
        gap = gap / 3 + 1; /* 每次都是跳的按间隔的排序 */

        for(i = gap; i < len; i+=gap)
        {
            k = i;
            temp = array[k];
            for(j = i-gap; (j>=0)&&(array[j]>temp); j-=gap)/* 插入排序这里做了一次交换 */
            {
                array[j+gap] = array[j]; /* 后移gap个单位 */
                k = j;
            }
            array[k] = temp; /* 插入temp */
        }

    }while(gap > 1);
}
int main(void)
{
    int array[] = {21, 25, 49, 25, 16,8};
    int len = sizeof(array) / sizeof(*array);

    printLn(array, len);

    ShellSort(array, len);

    printLn(array, len);


    return 0;
}

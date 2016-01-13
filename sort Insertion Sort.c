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

void InsertSort(int array[], int len) // O(n*n)
{
    int i = 0;
    int j = 0;
    int new_node = -1;

    for(i = 1; i < len; ++i)
    {
        new_node = array[i];
        for(j = i-1; j>=0; j--)
        {
            if(array[j] > new_node){ // 如果当前位置的元素 大于新的元素值 向后移动 直到不大于的时候在其后面插入新元素
                array[j+1] = array[j];
            }
            else
                break;
        }
        array[j+1] = new_node; /* 由于每次j的for循环结束会j--所以这里插入的应该是i+1的位置 */
    }
}
int main(void)
{
    int array[] = {21, 25, 49, 25, 16,8};
    int len = sizeof(array) / sizeof(*array);

    printLn(array, len);

    InsertSort(array, len);

    printLn(array, len);


    return 0;
}

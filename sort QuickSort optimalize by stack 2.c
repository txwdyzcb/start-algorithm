#include <iostream>
#include <cassert>
using namespace std;

void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
int partition(int array[], int low, int high)
{
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

void QSort(int arr[], int start, int end)
{
    assert(start < end);

    int low = start;
    int high = end;
    int curr = 0;
    int mid;

    int stack[high-low+1];

    stack[curr] = low;  // 有点类似于栈 curr 指向数组尾 通过curr来存取数 实际上是一个非常简洁的栈
    ++curr;             // 这个精简栈用的很好
    stack[curr] = high;
    ++curr;

    while( curr != 0 )
    {
        --curr;
        high = stack[curr];
        --curr;
        low = stack[curr];

        if(low < high)
        {
            mid = partition(arr, low, high);
            if ( mid - low > high - mid ) // 不明白为什么这里要将 长度较长的部分先压入 这样取出的部分 就是长度比较小的
            {
                stack[curr] = low;
                ++curr;
                stack[curr] = mid - 1;
                ++curr;
                if ( high > mid )
                {
                    stack[curr] = mid + 1;
                    ++curr;
                    stack[curr] = high;
                    ++curr;
                }
            }
            else
            {
                stack[curr] = mid + 1;
                ++curr;
                stack[curr] = high;
                ++curr;
                if ( mid > high )
                {
                    stack[curr] = low;
                    ++curr;
                    stack[curr] = mid - 1;
                    ++curr;
                }
            }
        }
    }
}

int main()
{
    int arr[5] = {1, 5, 4, 23, 0};

    QSort(arr, 0, 4);

    int i;

    for(i = 0; i < 5; i++)
    {
        cout<<arr[i]<<endl;
    }

    return 0;
}

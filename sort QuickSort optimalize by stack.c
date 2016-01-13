#include <iostream>
#include <cassert>
#include <stack>
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
    int mid;

    stack<int> stk;

    stk.push(low);
    stk.push(high);

    while(!stk.empty()){
        high = stk.top();
        stk.pop();
        low  = stk.top();
        stk.pop();

        if(low < high){
            mid = partition(arr, low, high);

            if(mid - low > high - mid){ // ps: 压入子文件中的较大者 确保栈的最大深度为lgN(2为底)
                if(low < mid){
                    stk.push(low);
                    stk.push(mid - 1);
                }
                if(mid < high){
                    stk.push(mid + 1);
                    stk.push(high);
                }
            }
            else{
                if(mid < high){
                    stk.push(mid + 1);
                    stk.push(high);
                }
                if(low < mid){
                    stk.push(low);
                    stk.push(mid - 1);
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
        cout << arr[i] <<" ";
    }

    return 0;
}

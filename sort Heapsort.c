#include <iostream>
using namespace std;

// 父节点i的左子节点在位置 (2*i+1);
// 父节点i的右子节点在位置 (2*i+2);
// 子节点i的父节点在位置 floor((i-1)/2);

void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

// 筛选算法 自顶向下 还有一种是自底向上
void sift(int array[], int ind, int len)
{
    int node = ind;
    int left  = node * 2 + 1;
    int right;
    int child;

    while(left < len)// 未筛选到叶子节点
    {
        // 如果要筛选的节点既有左孩子又有右孩子并且左孩子值小于右孩子 从二者中选出较大的并记

        right = left + 1;
        child = (right < len &&  array[left] <  array[right]) ? right : left;

        // 如果要筛选的节点中的值大于左右孩子的较大者则退出
        if( array[node] >  array[child])
            break;
        else
        {
            swap(array, child, node);

            node = child; // 重置要筛选的节点和要筛选的左孩子
            left = 2 * node  + 1;
        }
    }
    return;
}

void heap_sort(int d[], int n)
{
    //1.初始化建堆, i从最后一个非叶子节点开始
    for(int i = (n - 2) / 2; i >= 0; i--)
        sift(d, i, n);

    for(int j = 0; j < n; j++)
    {
        swap(d, 0, n - j -1);// 2.交换堆首和堆尾

        //3.筛选编号为0 把堆的尺寸缩小1，并调用shift_down(0),目的是把新的数组顶端数据调整到相应位置
        sift(d, 0, n - j - 1);
    }
}

// 原地堆排序的实现
int main()
{
    int a[] = {3, 5, 3, 6, 4, 7, 5, 7, 4}; // QQ

    heap_sort(a, sizeof(a) / sizeof(*a));

    int size = sizeof(a) / sizeof(*a);
    for(int i = 0; i < size; i++)
    {
        cout << a[i] << ' ';
    }
    cout << endl;
    return 0;
}

#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <math.h>
using namespace std;

// queue[] 就是一个hash
void VecToDeque(vector<int>&vec,queue<int> deq[],int power)
{
    vector<int>::iterator it;
    for(it=vec.begin();it!=vec.end();it++) {
        int itor=(*it) / power % 10;
        deq[itor].push(*it);
    }
}

// 2. 将队列中按照某位 排好的数拷贝到容器中
void DeqToVec(vector<int>&vect,queue<int>deq[])
{
    int count=0;
    for(int i=0;i<10;i++)             // 队列组
    {
        while(!deq[i].empty())
        {
            vect[count]=deq[i].front();
            count++;
            deq[i].pop();
        }
    }
}

int Max(vector<int>&vec)  //求队列中的最大数的位数
{
    vector<int>::iterator it;
    it=vec.begin();
    int max=*it;
    for(it=vec.begin();it!=vec.end();it++)  // 得到最大数
    {
        if(max<*it){
            max=*it;
        }
    }
    int count=1;
    while(max>=10)  // 得到最大位数
    {
        max/=10;
        count++;
    }
    return
        count;
}
void RadixLSDSort(vector<int>&vec)  // 3.调用VecToDeque和Colect对数字进行排序
{
    int count=Max(vec);
    int power=1;
    queue<int>que[10]; // 0-9

    for(int i=1;i<=count;i++)
    {
        VecToDeque(vec,que,power);  // 每次执行这两部操作将vector中元素按照取位放入deque中
        DeqToVec(vec,que); // 再将某位排好的拷贝到vector中 完成按照某位的一次排序 这里power作为队列组的索引
        power *= 10; // LSD从低位开始
    }
}
void RadixMSDSort(vector<int>&vec)  // 3.调用VecToDeque和Colect对数字进行排序
{
    int count=Max(vec);
    int power= pow(10, count);
    queue<int>que[10]; // 0-9

    for(int i = count; i >= 1; --i)
    {
        VecToDeque(vec,que,power);  // 每次执行这两部操作将vector中元素按照取位放入deque中
        DeqToVec(vec,que); // 再将某位排好的拷贝到vector中 完成按照某位的一次排序 这里power作为队列组的索引
        power /= 10; // LSD从低位开始
    }
}
int main()
{
    vector<int>vec;

    for(int i=0;i<10;i++){
        vec.push_back(rand()%99);
    }

    vector<int>::iterator it;
    for(it=vec.begin();it!=vec.end();it++) {
        cout<<*it<<" ";
    }
    cout<<endl;

    RadixMSDSort(vec);
    for(it=vec.begin();it!=vec.end();it++)
    {
        cout<<*it<<" ";
    }
    return 0;
}

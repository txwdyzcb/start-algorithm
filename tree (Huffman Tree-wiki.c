//以下為C++程式碼，在GCC下編譯通過
//僅用於示範如何根據權值構建霍夫曼樹，
//沒有經過性能上的優化及加上完善的異常處理。
#include <cstdlib>
#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;

struct node {                               //霍夫曼樹節點結構

    unsigned key;                           //保存权值
    node* lchild;                           //左孩子指針
    node* rchild;                           //右孩子指針
};

bool compare( node* a, node* b)
{
    return a->key < b->key;
}
int main()
{
    deque<node*> forest;                       // 使用2个队列的实现  这个队列用来存储N个节点
    deque<int> sum;                           // 此處也可使用bitset 这个队列用来存储两两权重的和

    node* ptr;
    int frequency[]={2, 3, 4, 4, 5, 7}; // 每个节点的权值
    const int size = sizeof(frequency) / sizeof(frequency[0]);

    // 1.将N个节点按权值的大小 压入forest队列中
    for(int i=0;i<size;i++)
    {
        ptr=new node;
        ptr->key=frequency[i];
        ptr->lchild=NULL;
        ptr->rchild=NULL;

        forest.push_back(ptr);
    }

    // 2.构建霍夫曼树
    for(int i=0; i < size-1; i++) // 注意这里需要的次数为 size-1
    {
        // 3.按照权值排序
        sort(forest.begin(),forest.end(),compare);
        ptr=new node;

        // 4.将队列中权值最小的2个节点弹出 合成新的节点
        ptr->key=forest[0]->key + forest[1]->key;
        ptr->lchild=forest[0];
        ptr->rchild=forest[1];

        forest.pop_front();
        forest.pop_front();
        forest.push_back(ptr);

        sum.push_back(ptr->key);
    }

    ptr = forest.front(); // 最后一个在队列的节点为根节点
    cout << "root : " << ptr->key << endl;

    for (int i=0; i < sum.size(); i++){
        cout << sum[i] << " ";
    }
    cout << endl;

    return 0;
}
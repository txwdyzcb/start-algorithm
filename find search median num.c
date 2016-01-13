//  1.在数组中随机找个元素，将比它大的放在它后面比它小的放在它前面，进行排列
//  2.返回排列后它的位置， 将此时的位置与要找的位置即(中位)，作对比，
//    如果相等 找到
//    如果小于 在高位中继续递归查找
//    如果大于 在低位中继续递归查找   

#include<iostream> 
using namespace std;  

//将第1个元素移动到指定位置，且保证小于此元素的在其左边，大于的在其右边  
static int get_middle(int *pArray,int start,int end)  
{  
    if(start >= end)  
        return start;  
    int midvalue = pArray[start];  
    int save_ptr = start;  
    int curr = start;  

    while(curr <= end)  
    {  
        if(pArray[curr] < midvalue)             // 如果当前值小于midvalue 将其移动到midvalue前
        {  
            pArray[save_ptr] = pArray[curr];  
            pArray[curr] = pArray[save_ptr+1];  // 将save_ptr+1即它下一个位置的值保持到curr中
            save_ptr++;                         // 指针右移
        }  
        curr++;  
    }  
    pArray[save_ptr] = midvalue;                // 把坑填上
    return save_ptr;  
}  

int get_random(int start, int end)
{
    return (rand() % (end-start+1) + start);    
}
void swap(int* xx, int* yy)
{
    int *temp;
    temp = xx;
    xx = yy;
    yy = temp;
}
  
int get_middle_random(int *pArray,int start,int end)  
{  
    int mid = get_random(start,end);  
    swap(pArray+start,pArray+mid);                  // 1. 随机化方法，查找随机位置，与第1位交换。

    return get_middle(pArray,start,end);            // 2. 以某个数为界将数组排列 并且返回它的位置
}  

int _quick_search(int *pArray,int start,int end,int pos)  
{  
    if(start >= end)  
        return pArray[end];  

    int mid = get_middle_random(pArray,start,end); 

    if(mid == pos)                                  // 找到  
        return pArray[mid]; 
    else if(pos < mid)                              // pos在低位  
        return _quick_search(pArray,start,mid-1,pos);  
    else                                            // pos在高位  
        return _quick_search(pArray,mid+1,end,pos);  
}  

int QuickSearch(int *pArray,int cnt,int pos)  
{  
    if(cnt <= 0) {                                    // 判断参数的合法性    
        return 0;  
    }  
    if(pos > cnt)  
        pos = cnt;  

    return _quick_search(pArray,0,cnt-1,pos-1);     // 数组从0开始
}  

int main()
{
    int a[] = {1, 2, 5, 0, 8, 3, 9};
    int size = sizeof(a) / sizeof(a[0]);
    int mid;
    
    if(size % 2 != 0)
    {
        mid = size / 2 + 1;
    }
    else
        mid = size /2;

    cout<<QuickSearch(a, size, mid)<<endl;

    return 0;
}
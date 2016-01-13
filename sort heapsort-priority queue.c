#include <stdio.h>
#include <malloc.h>

#define MinPQSize (10)
#define MinData (-32767)

struct HeapStruct
{
    int Capacity;
    int Size;
    int *Elements;
};

HeapStruct*
Initialize( int MaxElements )
{
    HeapStruct* H;

    if ( MaxElements < MinPQSize ){
        return NULL;
    }

    H = (HeapStruct*)malloc( sizeof( struct HeapStruct ) );
    if ( H == NULL ){
        return H;
    }

    H->Elements = (int*)malloc((MaxElements + 1 ) * sizeof(int)); // 多分配一个[0] 哨兵节点
    if ( H->Elements == NULL ){
        free(H);
        return NULL;
    }

    H->Capacity = MaxElements;
    H->Size = 0;
    H->Elements[0] = MinData; // [0] 哨兵节点

    return H;
}

void
Destroy( HeapStruct* H ){
    free( H->Elements );
    free( H );
}

void MakeEmpty( HeapStruct* H ){    H->Size = 0; }

int IsEmpty( HeapStruct* H ){    return H->Size == 0; }

int IsFull( HeapStruct* H ){    return H->Size == H->Capacity; }

int
FindMin( HeapStruct* H ){
    return (!IsEmpty(H)) ? H->Elements[0] : H->Elements[1];
}

// H->Elements[ 0 ] is a sentinel [0]是哨兵节点
void
Insert( int X, HeapStruct* H ){
    int i;

    if (IsFull(H)){
        return ;
    }

    // 父节点上虑
    // 建立一个空的位置 如果满足堆序性 插入节点X 否则将父节点插入该位置 删除父节点 新位置上移
    // 最小堆
    // 自底向上
    for ( i = ++ H->Size; H->Elements[ i / 2 ] > X; i /= 2) {
        H->Elements[ i ] = H->Elements[ i / 2 ];
    }

    H->Elements[ i ] = X;
}

// DeleteMin， 删除最小元，即二叉树的根或父节点。删除该节点元素后，队列最后一个元素必须移动到堆得某个位置，
// 使得堆仍然满足堆序性质。这种向下替换元素的过程叫作下滤
int
DeleteMin( HeapStruct* H )
{
    int i, child;
    int MinElement, LastElement;

    if ( IsEmpty( H ) ){
        return H->Elements[ 0 ]; // 哨兵节点0
    }

    MinElement = H->Elements[ 1 ];
    LastElement = H->Elements[ H->Size-- ];

    // 自顶向下
    for ( i = 1; i * 2 <= H->Size; i = child ){
        child = i * 2;

        if ( child != H->Size && H->Elements[ child + 1 ] < H->Elements[ child ] ){ // 右节点 < 左节点 则孩子为右节点
            child++;
        }

        if ( LastElement > H->Elements[child] )
            H->Elements[ i ] = H->Elements[ child ];
        else
            break;
    }

    H->Elements[ i ] = LastElement;
    return MinElement;
}

int
main()
{
    HeapStruct* H = Initialize(50);
    int ar[] = { 32, 21, 16, 24, 31, 19, 68, 65, 26, 13 };
    int i;

    for ( i = 0; i < 10; i++ ){
        Insert( ar[i], H );
    }

    for ( i = 0; i < 10; i++ ){
        printf( "%d\n", DeleteMin( H ) );
    }
    return 0;
}

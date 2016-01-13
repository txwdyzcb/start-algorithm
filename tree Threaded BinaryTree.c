#ifndef _SEQLIST_H_
#define _SEQLIST_H_

typedef void SeqList;
typedef void SeqListNode;

SeqList* SeqList_Create(int capacity);

void SeqList_Destroy(SeqList* list);

void SeqList_Clear(SeqList* list);

int SeqList_Length(SeqList* list);

int SeqList_Capacity(SeqList* list);

int SeqList_Insert(SeqList* list, SeqListNode* node, int pos);

SeqListNode* SeqList_Get(SeqList* list, int pos);

SeqListNode* SeqList_Delete(SeqList* list, int pos);

#endif


#include <stdio.h>
#include <malloc.h>
#include "SeqList.h"

typedef unsigned int TSeqListNode;

typedef struct _tag_SeqList
{
    int capacity;
    int length;
    TSeqListNode* node;
} TSeqList;

SeqList* SeqList_Create(int capacity) // O(1)
{
    TSeqList* ret = NULL;
    
    if( capacity >= 0 )
    {
        ret = (TSeqList*)malloc(sizeof(TSeqList) + sizeof(TSeqListNode) * capacity);
    }
    
    if( ret != NULL )
    {
        ret->capacity = capacity;
        ret->length = 0;
        ret->node = (TSeqListNode*)(ret + 1);
    }
    
    return ret;
}

void SeqList_Destroy(SeqList* list) // O(1)
{
    free(list);
}

void SeqList_Clear(SeqList* list) // O(1)
{
    TSeqList* sList = (TSeqList*)list;
    
    if( sList != NULL )
    {
        sList->length = 0;
    }
}

int SeqList_Length(SeqList* list) // O(1)
{
    TSeqList* sList = (TSeqList*)list;
    int ret = -1;
    
    if( sList != NULL )
    {
        ret = sList->length;
    }
    
    return ret;
}

int SeqList_Capacity(SeqList* list) // O(1)
{
    TSeqList* sList = (TSeqList*)list;
    int ret = -1;
    
    if( sList != NULL )
    {
        ret = sList->capacity;
    }
    
    return ret;
}

int SeqList_Insert(SeqList* list, SeqListNode* node, int pos) // O(n) 
{
    TSeqList* sList = (TSeqList*)list;
    int ret = (sList != NULL);
    int i = 0;
    
    ret = ret && (sList->length + 1 <= sList->capacity);
    ret = ret && (0 <= pos);
    
    if( ret )
    {
        if( pos >= sList->length )
        {
            pos = sList->length;
        }
        
        for(i=sList->length; i>pos; i--)
        {
            sList->node[i] = sList->node[i-1];
        }
        
        sList->node[i] = (TSeqListNode)node;
        
        sList->length++;
    }
    
    return ret;
}

SeqListNode* SeqList_Get(SeqList* list, int pos) // O(1) 
{
    TSeqList* sList = (TSeqList*)list;
    SeqListNode* ret = NULL;
    
    if( (sList != NULL) && (0 <= pos) && (pos <= sList->length) )
    {
        ret = (SeqListNode*)(sList->node[pos]);
    }
    
    return ret;
}

SeqListNode* SeqList_Delete(SeqList* list, int pos) // O(n)
{
    TSeqList* sList = (TSeqList*)list;
    SeqListNode* ret = SeqList_Get(list, pos);
    int i = 0;
    
    if( ret != NULL )
    {
        for(i=pos+1; i<sList->length; i++)
        {
            sList->node[i-1] = sList->node[i];
        }
        
        sList->length--;
    }
    
    return ret;
}


#ifndef _BTREE_H_
#define _BTREE_H_

#define BT_LEFT 0
#define BT_RIGHT 1

typedef void BTree;
typedef unsigned long long BTPos;

typedef struct _tag_BTreeNode BTreeNode;
struct _tag_BTreeNode
{
    BTreeNode* left;
    BTreeNode* right;
};

typedef void (BTree_Printf)(BTreeNode*);

BTree* BTree_Create();

void BTree_Destroy(BTree* tree);

void BTree_Clear(BTree* tree);

int BTree_Insert(BTree* tree, BTreeNode* node, BTPos pos, int count, int flag);

BTreeNode* BTree_Delete(BTree* tree, BTPos pos, int count);

BTreeNode* BTree_Get(BTree* tree, BTPos pos, int count);

BTreeNode* BTree_Root(BTree* tree);

int BTree_Height(BTree* tree);

int BTree_Count(BTree* tree);

int BTree_Degree(BTree* tree);

void BTree_Display(BTree* tree, BTree_Printf* pFunc, int gap, char div);

#endif



#include <stdio.h>
#include <malloc.h>
#include "BTree.h"

typedef struct _tag_BTree TBTree;
struct _tag_BTree
{
    int count;
    BTreeNode* root;
};

static void recursive_display(BTreeNode* node, BTree_Printf* pFunc, int format, int gap, char div) // O(n)
{
    int i = 0;
    
    if( (node != NULL) && (pFunc != NULL) )
    {
        for(i=0; i<format; i++)
        {
            printf("%c", div);
        }
        
        pFunc(node);
        
        printf("\n");
        
        if( (node->left != NULL) || (node->right != NULL) )
        {
            recursive_display(node->left, pFunc, format + gap, gap, div);
            recursive_display(node->right, pFunc, format + gap, gap, div);
        }
    }
    else
    {
        for(i=0; i<format; i++)
        {
            printf("%c", div);
        }
        printf("\n");
    }
}

static int recursive_count(BTreeNode* root) // O(n)
{
    int ret = 0;
    
    if( root != NULL )
    {
        ret = recursive_count(root->left) + 1 + recursive_count(root->right);
    }
    
    return ret;
}

static int recursive_height(BTreeNode* root) // O(n)
{
    int ret = 0;
    
    if( root != NULL )
    {
        int lh = recursive_height(root->left);
        int rh = recursive_height(root->right);
        
        ret = ((lh > rh) ? lh : rh) + 1;
    }
    
    return ret;
}

static int recursive_degree(BTreeNode* root) // O(n)
{
    int ret = 0;
    
    if( root != NULL )
    {
        if( root->left != NULL )
        {
            ret++;
        }
        
        if( root->right != NULL )
        {
            ret++;
        }
        
        if( ret == 1 )
        {
            int ld = recursive_degree(root->left);
            int rd = recursive_degree(root->right);
            
            if( ret < ld )
            {
                ret = ld;
            }
            
            if( ret < rd )
            {
                ret = rd;
            }
        }
    }
    
    return ret;
}

BTree* BTree_Create() // O(1)
{
    TBTree* ret = (TBTree*)malloc(sizeof(TBTree));
    
    if( ret != NULL )
    {
        ret->count = 0;
        ret->root = NULL;
    }
    
    return ret;
}

void BTree_Destroy(BTree* tree) // O(1)
{
    free(tree);
}

void BTree_Clear(BTree* tree) // O(1)
{
    TBTree* btree = (TBTree*)tree;
    
    if( btree != NULL )
    {
        btree->count = 0;
        btree->root = NULL;
    }
}

int BTree_Insert(BTree* tree, BTreeNode* node, BTPos pos, int count, int flag) // O(n) 
{
    TBTree* btree = (TBTree*)tree;
    int ret = (btree != NULL) && (node != NULL) && ((flag == BT_LEFT) || (flag == BT_RIGHT));
    int bit = 0;
    
    if( ret )
    {
        BTreeNode* parent = NULL;
        BTreeNode* current = btree->root;
        
        node->left = NULL;
        node->right = NULL;
        
        while( (count > 0) && (current != NULL) )
        {
            bit = pos & 1;
            pos = pos >> 1;
            
            parent = current;
            
            if( bit == BT_LEFT )
            {
                current = current->left;
            }
            else if( bit == BT_RIGHT )
            {
                current = current->right;
            }
            
            count--;
        }
        
        if( flag == BT_LEFT )
        {
            node->left = current;
        }
        else if( flag == BT_RIGHT )
        {
            node->right = current;
        }
        
        if( parent != NULL )
        {
            if( bit == BT_LEFT )
            {
                parent->left = node;
            }
            else if( bit == BT_RIGHT )
            {
                parent->right = node;
            }
        }
        else
        {
            btree->root = node;
        }
        
        btree->count++;
    }
    
    return ret;
}

BTreeNode* BTree_Delete(BTree* tree, BTPos pos, int count) // O(n)
{
    TBTree* btree = (TBTree*)tree;
    BTreeNode* ret = NULL; 
    int bit = 0;
    
    if( btree != NULL )
    {
        BTreeNode* parent = NULL;
        BTreeNode* current = btree->root;
        
        while( (count > 0) && (current != NULL) )
        {
            bit = pos & 1;
            pos = pos >> 1;
            
            parent = current;
            
            if( bit == BT_LEFT )
            {
                current = current->left;
            }
            else if( bit == BT_RIGHT )
            {
                current = current->right;
            }
            
            count--;
        }
        
        if( parent != NULL )
        {
            if( bit == BT_LEFT )
            {
                parent->left = NULL;
            }
            else if( bit == BT_RIGHT )
            {
                parent->right = NULL;
            }
        }
        else
        {
            btree->root = NULL;
        }
        
        ret = current;
        
        btree->count = btree->count - recursive_count(ret);
    }
    
    return ret;
}

BTreeNode* BTree_Get(BTree* tree, BTPos pos, int count) // O(n)
{
    TBTree* btree = (TBTree*)tree;
    BTreeNode* ret = NULL; 
    int bit = 0;
    
    if( btree != NULL )
    {
        BTreeNode* current = btree->root;
        
        while( (count > 0) && (current != NULL) )
        {
            bit = pos & 1;
            pos = pos >> 1;
            
            if( bit == BT_LEFT )
            {
                current = current->left;
            }
            else if( bit == BT_RIGHT )
            {
                current = current->right;
            }
            
            count--;
        }
        
        ret = current;
    }
    
    return ret;
}

BTreeNode* BTree_Root(BTree* tree) // O(1)
{
    TBTree* btree = (TBTree*)tree;
    BTreeNode* ret = NULL;
    
    if( btree != NULL )
    {
        ret = btree->root;
    }
    
    return ret;
}

int BTree_Height(BTree* tree) // O(n)
{
    TBTree* btree = (TBTree*)tree;
    int ret = 0;
    
    if( btree != NULL )
    {
        ret = recursive_height(btree->root);
    }
    
    return ret;
}

int BTree_Count(BTree* tree) // O(1)
{
    TBTree* btree = (TBTree*)tree;
    int ret = 0;
    
    if( btree != NULL )
    {
        ret = btree->count;
    }
    
    return ret;
}

int BTree_Degree(BTree* tree) // O(n)
{
    TBTree* btree = (TBTree*)tree;
    int ret = 0;
    
    if( btree != NULL )
    {
        ret = recursive_degree(btree->root);
    }
    
    return ret;
}

void BTree_Display(BTree* tree, BTree_Printf* pFunc, int gap, char div) // O(n)
{
    TBTree* btree = (TBTree*)tree;
    
    if( btree != NULL )
    {
        recursive_display(btree->root, pFunc, 0, gap, div);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "BTree.h"
#include "SeqList.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct Node
{
    BTreeNode header;
    char v;
};

void printf_data(BTreeNode* node)
{
    if( node != NULL )
    {
        printf("%c", ((struct Node*)node)->v);
    }
}

// 方法1：利用结点中的空指针域，使其指向后继结点 
// 前序遍历 若p不为空，将p->left指向当前结点，并将p置为NULL 
//          若当前结点的左子树为空时，将p指向当前结点
void thread_via_left(BTreeNode* root, BTreeNode** pp)
{
    if( (root != NULL) && (pp != NULL) )
    {
        if( *pp != NULL )
        {
            (*pp)->left = root;
            *pp = NULL;
        }
        
        if( root->left == NULL )
        {
            *pp = root;
        }
        
        thread_via_left(root->left, pp);
        thread_via_left(root->right, pp);
    }
}

// 方法2: 遍历二叉树 将所有节点保存到一个容器中
void thread_via_list(BTreeNode* root, SeqList* list)
{
    if( (root != NULL) && (list != NULL) )
    {
        SeqList_Insert(list, (SeqListNode*)root, SeqList_Length(list));
        
        thread_via_list(root->left, list);
        thread_via_list(root->right, list);
    }
}

int main(int argc, char *argv[])
{
    BTree* tree = BTree_Create();
    BTreeNode* current = NULL;
    BTreeNode* p = NULL;
    SeqList* list = NULL;
    int i = 0;
    
    struct Node n1 = {{NULL, NULL}, 'A'};
    struct Node n2 = {{NULL, NULL}, 'B'};
    struct Node n3 = {{NULL, NULL}, 'C'};
    struct Node n4 = {{NULL, NULL}, 'D'};
    struct Node n5 = {{NULL, NULL}, 'E'};
    struct Node n6 = {{NULL, NULL}, 'F'};
    
    BTree_Insert(tree, (BTreeNode*)&n1, 0, 0, 0);
    BTree_Insert(tree, (BTreeNode*)&n2, 0x00, 1, 0);
    BTree_Insert(tree, (BTreeNode*)&n3, 0x01, 1, 0);
    BTree_Insert(tree, (BTreeNode*)&n4, 0x00, 2, 0);
    BTree_Insert(tree, (BTreeNode*)&n5, 0x02, 2, 0);
    BTree_Insert(tree, (BTreeNode*)&n6, 0x02, 3, 0);
    
    printf("Full Tree: \n");
    
    BTree_Display(tree, printf_data, 4, '-');
    
    printf("Thread via List:\n");
    
    list = SeqList_Create(BTree_Count(tree));
    
    thread_via_list(BTree_Root(tree), list);
    
    for(i=0; i<SeqList_Length(list); i++)
    {
        printf("%c, ", ((struct Node*)SeqList_Get(list, i))->v);
    }
    
    printf("\n");
    
    printf("Thread via Left:\n");
    
    current = BTree_Root(tree);
    
    thread_via_left(current, &p);
    
    while( current != NULL )
    {
        printf("%c, ", ((struct Node*)current)->v);
        
        current = current->left;
    }
    
    printf("\n");
    
    BTree_Destroy(tree);
    
    return 0;
}



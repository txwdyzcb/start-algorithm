#ifndef _LINKQUEUE_H_
#define _LINKQUEUE_H_

typedef void LinkQueue;

LinkQueue* LinkQueue_Create();

void LinkQueue_Destroy(LinkQueue* queue);

void LinkQueue_Clear(LinkQueue* queue);

int LinkQueue_Append(LinkQueue* queue, void* item);

void* LinkQueue_Retrieve(LinkQueue* queue);

void* LinkQueue_Header(LinkQueue* queue);

int LinkQueue_Length(LinkQueue* queue);

#endif

#include <malloc.h>
#include <stdio.h>
#include "LinkQueue.h"

typedef struct _tag_LinkQueueNode TLinkQueueNode;
struct _tag_LinkQueueNode
{
    TLinkQueueNode* next;
    void* item;
};

typedef struct _tag_LinkQueue
{
    TLinkQueueNode* front;
    TLinkQueueNode* rear;
    int length;
} TLinkQueue;

LinkQueue* LinkQueue_Create() // O(1)
{
    TLinkQueue* ret = (TLinkQueue*)malloc(sizeof(TLinkQueue));
    
    if( ret != NULL )
    {
        ret->front = NULL;
        ret->rear = NULL;
        ret->length = 0;
    }
    
    return ret;
}

void LinkQueue_Destroy(LinkQueue* queue) // O(n)
{
    LinkQueue_Clear(queue);
    free(queue);
}

void LinkQueue_Clear(LinkQueue* queue) // O(n)
{
    while( LinkQueue_Length(queue) > 0 )
    {
        LinkQueue_Retrieve(queue);
    }
}

int LinkQueue_Append(LinkQueue* queue, void* item) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = (TLinkQueueNode*)malloc(sizeof(TLinkQueueNode));
    int ret = (sQueue != NULL ) && (item != NULL) && (node != NULL);
    
    if( ret )
    {
        node->item = item;
        
        if( sQueue->length > 0 )
        {
            sQueue->rear->next = node;
            sQueue->rear = node;
            node->next = NULL;
        }
        else
        {
            sQueue->front = node;
            sQueue->rear = node;
            node->next = NULL;
        }
        
        sQueue->length++;
    }
    
    if( !ret )
    {
        free(node);
    }
    
    return ret;
}

void* LinkQueue_Retrieve(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = NULL;
    void* ret = NULL;
    
    if( (sQueue != NULL) && (sQueue->length > 0) )
    {
        node = sQueue->front;
        
        sQueue->front = node->next;
        
        ret = node->item;
        
        free(node);
        
        sQueue->length--;
        
        if( sQueue->length == 0 )
        {
            sQueue->front = NULL;
            sQueue->rear = NULL;
        }
    }
    
    return ret;
}

void* LinkQueue_Header(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    void* ret = NULL;
    
    if( (sQueue != NULL) && (sQueue->length > 0) )
    {
        ret = sQueue->front->item;
    }
    
    return ret;
}

int LinkQueue_Length(LinkQueue* queue) // O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    int ret = -1;
    
    if( sQueue != NULL )
    {
        ret = sQueue->length;
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
#include "LinkQueue.h"

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

void pre_order_traversal(BTreeNode* root)
{
    if( root != NULL )
    {
        printf("%c, ", ((struct Node*)root)->v);
        
        pre_order_traversal(root->left);
        pre_order_traversal(root->right);
    }
}

void middle_order_traversal(BTreeNode* root)
{
    if( root != NULL )
    {
        middle_order_traversal(root->left);
        
        printf("%c, ", ((struct Node*)root)->v);
        
        middle_order_traversal(root->right);
    }
}

void post_order_traversal(BTreeNode* root)
{
    if( root != NULL )
    {
        post_order_traversal(root->left);
        
        post_order_traversal(root->right);
        
        printf("%c, ", ((struct Node*)root)->v);
    }
}

void level_order_traversal(BTreeNode* root)
{
    if( root != NULL )
    {
       LinkQueue* queue = LinkQueue_Create();
       
       if( queue != NULL )
       {
            LinkQueue_Append(queue, root); // 需要借助队列来实现广度优先的遍历
            
            while( LinkQueue_Length(queue) > 0 )
            {
                struct Node* node = (struct Node*)LinkQueue_Retrieve(queue);
                
                printf("%c, ", node->v);
                
                LinkQueue_Append(queue, node->header.left);
                LinkQueue_Append(queue, node->header.right);
            }
       }
       
       LinkQueue_Destroy(queue);
    }
}


int main(int argc, char *argv[])
{
    BTree* tree = BTree_Create();
    
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
    
    printf("Pre Order Traversal:\n");
    
    pre_order_traversal(BTree_Root(tree));
    
    printf("\n");
    
    printf("Middle Order Traversal:\n");
    
    middle_order_traversal(BTree_Root(tree));
    
    printf("\n");
    
    printf("Post Order Traversal:\n");
    
    post_order_traversal(BTree_Root(tree));
    
    printf("\n");
    
    printf("Level Order Traversal:\n");
    
    level_order_traversal(BTree_Root(tree));
    
    printf("\n");
    
    BTree_Destroy(tree);
    
    return 0;
}



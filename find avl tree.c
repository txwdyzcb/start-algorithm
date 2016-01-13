#ifndef _AVL_H_
#define _AVL_H_

typedef struct node
{
    struct node* left;
    struct node* right;
    struct node* parent;
    void* data;
    int num;
}node_t;

typedef struct avltree
{
    node_t* root;
}avltree_t;

typedef int (*CompareFunc)(void*, void*);

avltree_t* AVLTree_create(void);

void AVLTree_destroy(avltree_t* avltree);

void AVLTree_insert(avltree_t* avltree, void* data, CompareFunc compareFunc);

void AVLTree_delete(avltree_t* avltree, void* data, CompareFunc compareFunc);

void* AVLTree_find(avltree_t* avltree, void* data, CompareFunc compareFunc);


#endif // _AVL_H_

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

static void AVLTree_rotateLeft(avltree_t *avltree, node_t* ANode);
static void AVLTree_rotateRight(avltree_t *avltree, node_t* ANode);
static void AVLTree_rotateLeftAndRight(avltree_t *avltree, node_t* startNode);
static void AVLTree_rotateRightAndLeft(avltree_t *avltree, node_t* startNode);

static node_t* AVLTree_find_node(avltree_t* avltree, void* data, CompareFunc compareFunc);
static void AVLTree_FixBalance(node_t* startNode, void *data, CompareFunc compareFunc);
static void AVLTree_adjustForDelete(avltree_t* avltree, node_t* node, void* data, CompareFunc compareFunc);

avltree_t* AVLTree_create(void)
{
    avltree_t* avltree = (avltree_t*)malloc(sizeof(avltree_t));
    if(NULL == avltree)
        return NULL;

    avltree->root = NULL;

    return avltree;
}

void AVLTree_clear(node_t** node) // 这里要删除node节点由于本身类型为node_t*所以还需要再加一个*传入地址
{
    if(*node)
    {
        AVLTree_clear(&((*node)->left));
        AVLTree_clear(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
void AVLTree_destroy(avltree_t* avltree)
{
    if(avltree)
    {
        AVLTree_clear(&avltree->root);
        free(avltree);
        avltree = NULL;
    }
}

// 努力想把函数的代码控制在100行内 但是没办法 只能尽力写的有很强的逻辑了
void AVLTree_insert(avltree_t* avltree, void* data, CompareFunc compareFunc)
{
    if(NULL == avltree || NULL == data || NULL == compareFunc)
        return;

//  1.创建一个新节点
    node_t*  node = (node_t*)malloc(sizeof(node_t));
    if(!node)
        return;
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    node->num = 0;                       // 新节点初始化num为0

    node_t* curr = avltree->root;
    node_t* parentNode = NULL;
    node_t* ANode = NULL;                // 不平衡节点
    node_t* BNode = NULL;
    node_t* CNode = NULL;

    int ret = 0;
//  2.查找位置插入新节点 并且记下最后一个不平衡的节点
    while(curr)
    {
        parentNode = curr;
        if(1 == parentNode->num || -1 == parentNode->num)
            ANode = parentNode;

        ret = (*compareFunc)(data, curr->data);
        if(ret < 0)                         // 小在左边
            curr = curr->left;
        else if(ret > 0)
            curr = curr->right;
        else                                // 相等 说明存在了 不再插入 这里也可以处理成相等仍然插入
        {
            free(node);
            return;
        }
    } // end while
    node->parent = parentNode;
//  3.插入节点
    if(NULL == parentNode)                     // 树为空 插入root节点
        avltree->root = node;
    else                                       // 这样就插入了节点 接下来调整
    {
        if(ret < 0)
            parentNode->left = node;
        else
            parentNode->right = node;
    }

//  4.完成插入和调整
//  第一种：父节点num为0 不存在不平衡的节点 直接插入后再修改平衡因子即可
    if(NULL == ANode)
    {
        AVLTree_FixBalance(avltree->root, data, compareFunc); // 修改从根结点到插入节点的平衡因子
        return;
    }

//  第二种：不平衡节点num为1或者-1插入后变成了-2 0 2等情况的处理
    ret = (*compareFunc)(data, ANode->data);

//  1.ANode(1)   且在右插入 插入后变成0  2.ANode(-1)  且在左插入 插入后变成0
//  这2种情况插入后刚好使得ANode的num为0 所以不需要旋转 调整num就好 这些大家画图就很容易明白
    if((1 == ANode->num && ret > 0) || (-1 == ANode->num && ret < 0))
        AVLTree_FixBalance(avltree->root, data, compareFunc); // 修改从根结点到插入节点的平衡因子

//  3.ANode(1)   且插入在左 插入后Anode(2)
    else if(1 == ANode->num && ret < 0)
    {
        if((*compareFunc)(data, ANode->left->data) < 0)   // LL 插入在ANode的左子树的左边 需要右旋ANode 调整平衡因子
        {
            BNode = ANode->left;
            AVLTree_rotateRight(avltree, ANode);
            AVLTree_FixBalance(BNode, data, compareFunc);
            ANode->num = 0;
            BNode->num = 0;
        }
        else                                               // LR 插入在A节点左子树的右子树中
        {
            BNode = ANode->left;
            CNode = BNode->right;

            ret = (*compareFunc)(data, CNode->data);
            if(ret < 0)
                CNode->num += 1;                            // 左边插入CNode +1 右边插入-1
            else if(ret > 0)
                CNode->num -= 1;

            AVLTree_rotateLeftAndRight(avltree, ANode);       // 这里进行了 先左旋后右旋的操作

            if(ret < 0)
                AVLTree_FixBalance(BNode, data, compareFunc); // 调整平衡因子
            else if(ret > 0)
                AVLTree_FixBalance(ANode, data, compareFunc);
            else
            {}
        }
    }
//  4.ANode(-1) 且插入在右 插入后Anode(-2)
    else if(-1 == node->num && ret > 0)
    {
        if((*compareFunc)(data, ANode->right->data) > 0) // RR 左旋调整平衡因子
        {
            BNode = ANode->right;
            AVLTree_rotateLeft(avltree, ANode);
            AVLTree_FixBalance(BNode, data, compareFunc);
            ANode->num = 0;
            BNode->num = 0;
        }
        else                                               // RL
        {
            BNode = ANode->right;
            CNode = BNode->left;
            ret = (*compareFunc)(data, CNode->data);
            if(ret < 0)                             // 插入在CNode的左边CNode的num加+1
                CNode->num += 1;
            else if(ret > 0)
                CNode->num -= 1;

            AVLTree_rotateRightAndLeft(avltree, ANode);

            if(ret < 0)                          // 调整平衡因子
                AVLTree_FixBalance(ANode, data, compareFunc);
            else if(ret > 0)
                AVLTree_FixBalance(BNode, data, compareFunc);
        }
    }
}


void AVLTree_delete(avltree_t* avltree, void* data, CompareFunc compareFunc)
{
    assert(avltree && data && compareFunc);

    node_t* node = AVLTree_find_node(avltree, data, compareFunc);
    if(NULL == node)
        return;
    node_t* ANode;
    node_t* delNode;
// 1.node有2个节点
    if(node->left && node->right)
    {
        delNode = node->left; // 用左子树的最大值 替换node
        while(delNode->right)
            delNode = delNode->right;

        ANode = delNode->parent;
        node->data = delNode->data; // 和红黑树一样 我们将data拷贝到node中 避免指针的移动 提高效率
        if(delNode != node->left)
            ANode->right = delNode->left;
        else
            ANode->left = delNode->left;

        if(delNode->left)
            delNode->left->parent = ANode;

        if(delNode == ANode->left)
            ANode->num -= 1;
        else
            ANode->num += 1;

    }
// 2.处理最多有1个节点的情况
    else
    {
        ANode = node;
        if(node->left)                     // 只有左节点
        {
            delNode = node->left;
            node->data = delNode->data;
            node->left = NULL;
            ANode->num -= 1;
        }
        else if(node->right)            // 只有右节点
        {
            delNode = node->right;
            node->data = delNode->data;
            node->right = NULL;
            ANode->num += 1;
        }
        else           // 左右节点都不存在
        {
            ANode = node->parent;
            delNode = node;
            if(NULL == ANode)
            {
                avltree->root = ANode;
            }
            else if(ANode->left == node)
            {
                ANode->left = NULL;
                ANode->num -= 1;
            }
            else
            {
                ANode->right = NULL;
                ANode->num += 1;
            }
        }
    }
    if(delNode)                           // 删除节点
        free(delNode);

    // 调整平衡
    if(ANode)
        AVLTree_adjustForDelete(avltree, ANode, data, compareFunc);
}

void* AVLTree_find(avltree_t* avltree, void* data, CompareFunc compareFunc)
{
    assert(avltree && data && compareFunc);

    node_t* curr = avltree->root;

    while(curr)
    {
        if((*compareFunc)(data, curr->data) < 0)        // 小于在左边
        {
            curr = curr->left;
        }
        else if((*compareFunc)(data, curr->data) > 0)   // 大于在右边
        {
            curr = curr->right;
        }
        else                                            // 相等
        {
            return curr->data;
        }
    }
    return NULL;
}

static void AVLTree_rotateLeft(avltree_t *avltree, node_t* ANode)
{
    node_t* BNode;
    BNode = ANode->right;            // 这里A节点是 顶节点

    ANode->right = BNode->left;      // 1.将B节点的左节点变成A的右节点
    if(BNode && BNode->left)
        BNode->left->parent = ANode;

    BNode->parent = ANode->parent;   // 2.调整B节点和A节点的位置
    if(NULL == ANode->parent)        // root
    {
        BNode->parent = NULL;
        avltree->root = BNode;
    }
    else if(ANode->parent->left == ANode)
    {
        ANode->parent->left = BNode;
    }
    else
    {
        ANode->parent->right = BNode;
    }

    BNode->left = ANode;
    ANode->parent = BNode;
}
static void AVLTree_rotateRight(avltree_t *avltree, node_t* ANode)
{
    node_t* BNode;
    BNode = ANode->left;            // 这里A节点是 顶节点

    ANode->left = BNode->right;     // 1.将B节点的右节点变成A的左节点 因为它的右节点会变成A节点
    if(BNode && BNode->right)
    {
        BNode->right->parent = ANode;
    }

    BNode->parent = ANode->parent;  // 2.调整B节点和A节点的位置
    if(NULL == ANode->parent)       // root
    {
        BNode->parent = NULL;
        avltree->root = BNode;
    }
    else if(ANode->parent->left == ANode)
    {
        ANode->parent->left = BNode;
    }
    else
    {
        ANode->parent->right = BNode;
    }

    BNode->right = ANode;
    ANode->parent = BNode;
}

static void AVLTree_rotateLeftAndRight(avltree_t *avltree, node_t* startNode)
{
    node_t* ANode;
    node_t* BNode;
    node_t* CNode;

    ANode = startNode;
    BNode = ANode->left;
    CNode = BNode->right;

    AVLTree_rotateRight(avltree, BNode);
    AVLTree_rotateLeft(avltree, ANode);

    switch(CNode->num)
    {
        case 1:                // 插入在c的左子树
        ANode->num = 0;
        BNode->num = -1;
        break;
    case -1:                   // 插入在c的右子树
        ANode->num = 1;
        BNode->num = 0;
        break;
    default:                   // c就是插入节点
        ANode->num = 0;
        BNode->num = 0;
        break;
    }
    CNode->num = 0;
}
static void AVLTree_rotateRightAndLeft(avltree_t *avltree, node_t* startNode)
{
    node_t* ANode;
    node_t* BNode;
    node_t* CNode;

    ANode = startNode;
    BNode = ANode->left;
    CNode = BNode->right;

    AVLTree_rotateLeft(avltree, BNode);
    AVLTree_rotateRight(avltree, ANode);

    switch(CNode->num)
    {
        case 1:                // 插入在c的左子树
        ANode->num = -1;
        BNode->num = 0;
        break;
    case -1:                   // 插入在c的右子树
        ANode->num = 0;
        BNode->num = 1;
        break;
    default:                   // c就是插入节点
        ANode->num = 0;
        BNode->num = 0;
        break;
    }
    CNode->num = 0;
}

node_t* AVLTree_find_node(avltree_t* avltree, void* data, CompareFunc compareFunc)
{
    if(NULL == avltree || NULL == data || NULL == compareFunc)
        return NULL;

    node_t* curr = avltree->root;
    while(curr)
    {
        if((*compareFunc)(data, curr->data) < 0)        // 小于在左边
        {
            curr = curr->left;
        }
        else if((*compareFunc)(data, curr->data) > 0)   // 大于在右边
        {
            curr = curr->right;
        }
        else                                            // 相等
        {
            return curr;
        }
    }
    return NULL;
}
static void AVLTree_FixBalance(node_t* startNode, void *data, CompareFunc compareFunc)
{
    node_t* node;
    node_t* parentNode;
    int ret;

    node = startNode;
    while(node)
    {
        parentNode = node;
        ret = (*compareFunc)(data, node->data);
        if(ret < 0)
        {
            node = node->left;
            startNode->num += 1;
        }
        else if(ret > 0)
        {
            node = node->right;
            startNode->num -= 1;
        }
        else
            break;                           // 相同data break
    }
}

void AVLTree_adjustForDelete(avltree_t* avltree, node_t* node, void* data, CompareFunc compareFunc)
{
    node_t* ANode = node;
    node_t* parentNode;
    node_t* BNode;

    while(ANode)
    {
        switch(ANode->num)
        {
            case 0:
            if(ANode == avltree->root)
                break;
            else
            {
                parentNode = ANode->parent;
                if(ANode == parentNode->left)
                    parentNode->num -= 1;
                else
                    parentNode->num += 1;

                ANode = parentNode; // 将ANode指向它的父节点 继续循环调整它的父节点的不平衡情况
                continue;
            }
        case -1: case 1:
            break;
        case -2: // L不平衡情况 右边多了
            BNode = ANode->right;
            if(0 == BNode->num)
            {
                AVLTree_rotateLeft(avltree, ANode);
                ANode->num = -1;
                BNode->num = 1;
                break;
            }
            else if(-1 == BNode->num)
            {
                parentNode = ANode->parent;
                if(parentNode)
                {
                    if(ANode == parentNode->left) // 旋转后ANode如果是parent的left则会少1
                        parentNode->num -= 1;
                    else
                        parentNode->num += 1;
                }
                AVLTree_rotateLeft(avltree, ANode);
                ANode->num = 0;
                BNode->num = 0;

                ANode = parentNode;// 将ANode指向它的父节点 继续调整它的父节点的不平衡情况
            }
            else// 1
            {
                parentNode = ANode->parent;
                if(parentNode)
                {
                    if(ANode == parentNode->left) // 旋转后ANode如果是parent的left则会少1
                        parentNode->num -= 1;
                    else
                        parentNode->num += 1;
                }
                AVLTree_rotateRightAndLeft(avltree, ANode);
                ANode = parentNode;
            }
            continue;
        case 2: // R型不平衡 左太多
            BNode = ANode->left;
            if(0 == BNode->num)
            {
                AVLTree_rotateRight(avltree, ANode);
                ANode->num = 1;
                BNode->num = 11;
                break;
            }
            else if(-1 == BNode->num)
            {
                parentNode = ANode->parent;
                if(parentNode)
                {
                    if(ANode == parentNode->left) // 旋转后ANode如果是parent的left则会少1
                        parentNode->num -= 1;
                    else
                        parentNode->num += 1;
                }
                AVLTree_rotateLeftAndRight(avltree, ANode);

                ANode = parentNode;// 将ANode指向它的父节点 继续调整它的父节点的不平衡情况
            }
            else// 1
            {
                parentNode = ANode->parent;
                if(parentNode)
                {
                    if(ANode == parentNode->left) // 旋转后ANode如果是parent的left则会少1
                        parentNode->num -= 1;
                    else
                        parentNode->num += 1;
                }
                AVLTree_rotateRight(avltree, ANode);
                ANode->num = 0;
                BNode->num = 0;

                ANode = parentNode; // 将pANode 指向它的父节点，继续调整它的父节点的不平衡情况
            }
            continue; // loop while
        default:
            break;
        }
        break;
    }
}

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "avl.h"

#define CONVERT(m) (void*)&m

int compare(void* fir, void* sec)
{
    if(*((int*)fir) < *((int*)sec))
    {
        return -1;
    }
    else if(*((int*)fir) > *((int*)sec))
    {
        return 1;
    }
    else
        return 0;
}

int main(void)
{
    int arr[5] = {3, 2, 5, 6, 1};

    avltree_t* tree = AVLTree_create();

    AVLTree_insert(tree, CONVERT(arr[0]), compare);
    AVLTree_insert(tree, CONVERT(arr[1]), compare);
    AVLTree_insert(tree, CONVERT(arr[2]), compare);
    AVLTree_insert(tree, CONVERT(arr[3]), compare);
    AVLTree_insert(tree, CONVERT(arr[4]), compare);
    AVLTree_insert(tree, CONVERT(arr[3]), compare);

    AVLTree_delete(tree, CONVERT(arr[4]), compare);

    void* tmp = AVLTree_find(tree, CONVERT(arr[4]), compare);
    if(tmp)
    {
        printf("find %d \n", *((int*)tmp));
    }
    else
    {
        printf("not find");
    }

    AVLTree_destroy(tree);

    return 0;
}
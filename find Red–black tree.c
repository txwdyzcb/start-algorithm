#ifndef _RBTREE_H_
#define _RBTREE_H_

#define RED 0
#define BLACK 1

typedef struct node
{
    struct node* left;
    struct node* right;
    struct node* parent;
    void* data;
    int color;
}node_t;

typedef struct rbtree
{
    node_t* root;
}rbtree_t;

typedef int (*CompareFunc)(void*, void*);

rbtree_t* RBTree_create(void);

void RBTree_destroy(rbtree_t* rbtree);

void RBTree_insert(rbtree_t* rbtree, void* data, CompareFunc compareFunc);

void RBTree_delete(rbtree_t* rbtree, void* data, CompareFunc compareFunc);

void* RBTree_find(rbtree_t* rbtree, void* data, CompareFunc compareFunc);

#endif // _RBTREE_H_

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

static void RBTree_adjustColorForInsert(rbtree_t* rbtree, node_t* node);
static void RBTree_adjustColorForDelete(rbtree_t* rbtree, node_t* node);
static void RBTree_rotateLeft(rbtree_t *rbtree, node_t* ANode);
static void RBTree_rotateRight(rbtree_t *rbtree, node_t* ANode);
static inline node_t* RBTree_find_node(rbtree_t* rbtree, void* data, CompareFunc compareFunc);
static inline void RBTRee_swap_nodes(node_t** firstNode, node_t** secondNode);

rbtree_t* RBTree_create(void)
{
    rbtree_t* rbtree = (rbtree_t*)malloc(sizeof(rbtree_t));
    if(NULL == rbtree)
        return NULL;

    rbtree->root = NULL;

    return rbtree;
}
void RBTree_clear(node_t** node) // 这里要删除node节点由于本身类型为node_t*所以还需要再加一个*传入地址
{
    if(*node)
    {
        RBTree_clear(&((*node)->left));
        RBTree_clear(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
void RBTree_destroy(rbtree_t* rbtree)
{
    if(rbtree)
    {
        RBTree_clear(&rbtree->root);
        free(rbtree);
        rbtree = NULL;
    }
}

// 对于红黑树的插入和普通的二叉树搜索树一样小的左边 大的右边 然后调整颜色即可
// 颜色调整方面只有红红发生冲突时才需要调整颜色
// 调整颜色分2种：一种是叔叔为红色(循环改变颜色就好) 一种是叔叔为黑色(调整颜色加旋转)
void RBTree_insert(rbtree_t* rbtree, void* data, CompareFunc compareFunc)
{
    if(NULL == rbtree || NULL == data || NULL == compareFunc)
        return;

//  1.创建一个新节点
    node_t*  node = (node_t*)malloc(sizeof(node_t));
    if(!node)
        return;
    node->left = NULL;
    node->right = NULL;
    node->data = data;
    node->color = RED;                       // 新节点初始化为红色

    node_t* curr = rbtree->root;
    node_t* parent = NULL;
    int ret = 0;
//  2.查找空位置 插入
    while(curr)
    {
        parent = curr;
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

    node->parent = parent;                   // 插入

    if(NULL == node->parent)                      // 树为空 root颜色为黑色
    {
        node->color = BLACK;
        rbtree->root = node;
    }
    else
    {
        if(ret < 0)
            parent->left = node;
        else
            parent->right = node;
    }

    printf("insert data ok\n");

    if(node->parent && RED == node->parent->color) // 这里移动要先判断存在 再判断是否颜色为红色
    {
        RBTree_adjustColorForInsert(rbtree, node);
    }
}


// 红黑树的 删除节点 可以用左子树的最大节点或者右子树的最小节点来替换 然后进行颜色的调整
// 实际的操作仅仅将data拷贝到要删除节点的data 真正删除的是替换节点 并不做指针的交换 这样可以简化操作
void RBTree_delete(rbtree_t* rbtree, void* data, CompareFunc compareFunc)
{
    assert(rbtree && data && compareFunc);

//  找到要删除的节点
    node_t* delNode = RBTree_find_node(rbtree, data, compareFunc);
    if(NULL == delNode)
        return;

    node_t* parentNode = delNode->parent;
    node_t* tmpNode;
    node_t* delChild;

//  第一种情况：删除节点左右子节点都存在的情况 用左子树中最大节点替换
    if(delNode->left && delNode->right)
    {
        tmpNode = delNode->left;
        while(tmpNode->right)
            tmpNode = tmpNode->right;          // 循环结束 这里的tmpNode就是要替换的节点

        // 1.这里交换仅仅是将数据data拷贝 没有移动指针 这里不改变删除节点的颜色和指针仅仅是数据data达到交换的目的
        // 简化了操作
        delNode->data = tmpNode->data;
        //RBTRee_swap_nodes(&tmpNode, &delNode); // 原文的作者这么写的 我没这么写

        delChild = tmpNode->left;              // 2.将tmpNode(即最大左子节点) 用它自己的子节点代替
        delChild->parent = tmpNode->parent;
        if(tmpNode == tmpNode->parent->left)
            tmpNode->parent->left = delChild;
        else
            tmpNode->parent->right = delChild;

        // 如果替换节点为黑色 删除它后少了一个黑色节点 需要调整颜色和旋转
        // 如果替换节点是红色不需要调整颜色 直接删除就可以了 不影响 整个树
        if(tmpNode && BLACK == tmpNode->color)
                RBTree_adjustColorForDelete(rbtree, delChild);

        free(tmpNode); // 这里将tmpNode删除
    }

//  2.删除节点的左右节点都为空 即删除节点是叶子节点
    else if(NULL == delNode->left && NULL == delNode->right)
    {
        if(NULL == parentNode) // delNode是root结点
        {
            free(rbtree->root);
            rbtree->root = NULL;
        }
        else
        {
            if(delNode == delNode->parent->left)
                parentNode->left = NULL;
            else
                parentNode->right = NULL;

            delChild = NULL;
            delChild->parent = delNode->parent; // 它的父节点不为空
            tmpNode = delNode; // tmpNode保存了 删除节点 因为如果是黑色节点的话 删除需要旋转和改变颜色


            // 如果删除节点为黑色 需要对它对应的树 进行调整颜色和选择操作
            // 这里delChild为空不影响 因为它的父节点不为空 实际的操作中也对delChild不会照成什么影响 不会用到它
            if(BLACK == tmpNode->color)
                RBTree_adjustColorForDelete(rbtree, delChild);

            free(tmpNode);
        }
    }
//  3.删除节点只有一个子节点 直接用那个子节点替换
    else
    {
        if(delNode->left)
            delChild = delNode->left;
        else
            delChild = delNode->right;

        delChild->parent = parentNode;
        if(delNode == parentNode->left)
            parentNode->left = delChild;
        else
            parentNode->right = delChild;

        tmpNode = delNode;

        // 如果删除节点为黑色 需要对它对应的树 进行调整颜色和选择操作
        // 这里delChild为空不影响 因为它的父节点不为空 实际的操作中也对delChild不会照成什么影响 不会用到它
        if(BLACK == tmpNode->color)
            RBTree_adjustColorForDelete(rbtree, delChild);

        free(tmpNode);
    }
}

void* RBTree_find(rbtree_t* rbtree, void* data, CompareFunc compareFunc)
{
    assert(rbtree && data && compareFunc);

    node_t* curr = rbtree->root;

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
inline node_t* RBTree_find_node(rbtree_t* rbtree, void* data, CompareFunc compareFunc)
{
    if(NULL == rbtree || NULL == data || NULL == compareFunc)
        return NULL;

    node_t* curr = rbtree->root;
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

// 当红红发生冲突时需要改变颜色或者旋转
static void RBTree_adjustColorForInsert(rbtree_t* rbtree, node_t* node)
{
    while(node->parent && RED == node->parent->color)// node为红色 只有node的父节点为红色时发生冲突 才需要调整
    {
        node_t* nodeParent = node->parent;
        node_t* nodePaBro;
// 1.得到叔叔节点
        if(nodeParent->parent->left == nodeParent)
            nodePaBro = nodeParent->parent->right;
        else
            nodePaBro = nodeParent->parent->left;

// 第一种情况：红叔 不需要选择 循环调整颜色就好 见前面的示意图 比较简单自己话图一步一步就可以画出来
        if(nodePaBro && RED == nodePaBro->color)
        {
            nodeParent->color = BLACK;             // 调整颜色
            nodePaBro->color = BLACK;
            nodeParent->parent->color = RED;

            node = node->parent->parent;           // 这里node指向nodeparent->parent 可能红红冲突 需要循环调整
            if(NULL == node)                       // 到了根节点位置
            {
                rbtree->root->color = BLACK;
                return;
            }
        }

// 第二种情况：黑叔 需要旋转和调整颜色 见前面的示意图 比较简单自己话图一步一步就可以画出来
        else if(nodePaBro && nodeParent == nodeParent->parent->left && node == nodeParent->left)   // 1.LL
        {
            nodeParent->color = BLACK;
            nodeParent->parent->color = RED;

            RBTree_rotateRight(rbtree, nodeParent->parent);
            break;
        }
        else if(nodePaBro && nodeParent == nodeParent->parent->left && node == nodeParent->right)  // 2.LR
        {
            node = node->parent;
            RBTree_rotateLeft(rbtree, node);  // 对于LR情况 需要以父节点为旋转节点 循环旋转成LL类型
        }
        else if(nodePaBro && nodeParent == nodeParent->parent->right && node == nodeParent->left)  // 3.RL
        {
            node = node->parent;
            RBTree_rotateRight(rbtree, node); // 对于RL情况 需要以父节点为旋转节点 循环旋转成RR类型
        }
        else if(nodePaBro && nodeParent == nodeParent->parent->right && node == nodeParent->right) // 4.RR
        {
            nodeParent->color = BLACK;
            nodeParent->parent->color = RED;

            RBTree_rotateLeft(rbtree, node->parent->parent);
            break;
        }
    } // end while
    rbtree->root->color = BLACK;              // root 为黑色 可能在旋转中改变了它
}

static void RBTree_adjustColorForDelete(rbtree_t* rbtree, node_t* node)
{
    int flags;
    node_t* broNode = NULL;
    if(node && RED == node->color)
    {
        node->color = BLACK;
        return;
    }
    // 当删除节点为黑色时 如果子节点node为红色直接改成黑色即可 黑色时需要旋转 但是当前节点不参与
    // 这里的node可能为空 因为下面的操作不影响node 操作的是 node的父节点和兄弟节点
    while(node != rbtree->root && (BLACK == node->color || NULL == node))
    {
        if(node == node->parent->left)
        {
            flags = 0;
            broNode = node->parent->right;
        }
        else
        {
            flags = 1;
            broNode = node->parent->left;
        }

        // 1.parent(red) 无兄 子red 变成 子black
        if(NULL == broNode && node->parent && RED == node->parent->color && node->left)
        {
            node->left->color = RED;
        }
        // 2.parent(black) 兄(red)  变成 parent(red)  兄(black) 旋转父节点
        else if(broNode && RED == broNode->color)
        {
            node->parent->color = RED; // 这样经过旋转后一边不变 一边增加了一个黑色节点
            broNode->color =BLACK;
            if(0 == flags)
                RBTree_rotateLeft(rbtree, node->parent); // 左旋或者右旋 增加一个黑色的节点
            else
                RBTree_rotateRight(rbtree, node->parent);
        }
        // 3.Bro(black) BroL(black) BroR(black) 变成 兄(red) 子(黑) 父(黑) 向上遍历(黑父)
        else if(BLACK == broNode->left->color && BLACK == broNode->right->color)
        {
            broNode->color = RED;
            node->parent->color = BLACK;  // 将bro变成红色 父设为黑色 这样一边不变 一边增加了一个黑色节点 可以了
            node = node->parent;          // 由于父节点颜色改变了 需要循环向上更改
        }
        // 4.Bro(black) BroL(black) BroR(red) 变成 兄(父色) 父(黑色) 侄子(黑色) （子 黑色） 左旋父节点
        else if(BLACK == broNode->left->color && RED == broNode->right->color)
        {
            if(0 == flags)
            {
                broNode->color = node->parent->color;
                node->parent->color = BLACK;        // 这里的注释见5的注释 有很好的说明 自己一画就清楚了
                broNode->right->color = BLACK;
                node->color = BLACK;

                RBTree_rotateLeft(rbtree, node->parent);
                break;
            }
            else
            {
                node_t* parent = node->parent;
                broNode->right->color = node->parent->color;
                node->parent->color = BLACK;

                RBTree_rotateLeft(rbtree, broNode);
                RBTree_rotateRight(rbtree, parent);
                break;
            }
        }
        // 5.Bro(black) BroL(red)   BR(black) 变成 侄(父色) 父(黑色) 右旋转兄 左旋转父
        else if(RED == broNode->left->color && BLACK == broNode->right->color)
        {
            if(1 == flags)
            {
                broNode->color = node->parent->color; // 1.将父亲节点颜色保存到bro中 父节点设置为黑色
                node->parent->color = BLACK;
                node->color = BLACK;
                broNode->right->color = BLACK;

                RBTree_rotateRight(rbtree, node->parent); // 2.右旋后bro变成新的父节点 父节点颜色不变 右边多了一个黑色节点
                break;
            }
            else
            {
                node_t* parent = node->parent;
                broNode->left->color = node->parent->color;
                node->parent->color = BLACK;
                node->color = BLACK;
                broNode->left->color = BLACK;

                RBTree_rotateRight(rbtree, broNode); // 1.将broNode右旋这样右边多了一个节点(原来的broL节点) 颜色和父节点一样
                RBTree_rotateLeft(rbtree, parent);   // 2.将父节点左旋 broL节点变成父节颜色和原来一样 但是左边多了一个黑节点
                break;
            }
        }
        // 6.BroL(red)   BR(red) 转换成一黑一红 循环处理
        else if(RED == broNode->left->color && RED == broNode->right->color)
        {
            if(0 == flags)
                broNode->left->color = BLACK;
            else
                broNode->right->color = BLACK;
        }
    }
    if(node)
    {
        node->color = BLACK;  // 子 变成 black
    }
    return;
}
static void RBTree_rotateLeft(rbtree_t *rbtree, node_t* ANode)
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
        rbtree->root = BNode;
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
static void RBTree_rotateRight(rbtree_t *rbtree, node_t* ANode)
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
        rbtree->root = BNode;
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
static inline void RBTRee_swap_nodes(node_t** firstNode, node_t** secondNode) // 有问题
{
    void* tmp = (*firstNode)->data;
    (*firstNode)->data = (*secondNode)->data;
    (*secondNode)->data = tmp;
}

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "rbtree.h"

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
    rbtree_t* rbtree = RBTree_create();

    RBTree_insert(rbtree, CONVERT(arr[0]), compare);
    RBTree_insert(rbtree, CONVERT(arr[1]), compare);
    RBTree_insert(rbtree, CONVERT(arr[2]), compare);
    RBTree_insert(rbtree, CONVERT(arr[3]), compare);
    RBTree_insert(rbtree, CONVERT(arr[4]), compare);
    RBTree_insert(rbtree, CONVERT(arr[3]), compare);

    RBTree_delete(rbtree, CONVERT(arr[1]), compare);

    void* tmp = RBTree_find(rbtree, CONVERT(arr[2]), compare);
    if(tmp)
    {
        printf("find %d \n", *((int*)tmp));
    }

    RBTree_destroy(rbtree);

    return 0;
}

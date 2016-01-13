/**********************************************************
  可复用的静态链表

  *********************************************************/
#ifndef _STATIC_LIST_H_
#define _STATIC_LIST_H_

typedef void StaticList;
typedef void StaticListNode;

StaticList* StaticList_Create(int capacity);

void StaticList_Destroy(StaticList* list);

void StaticList_Clear(StaticList* list);

int StaticList_Length(StaticList* list);

int StaticList_Capacity(StaticList* list);

int StaticList_Insert(StaticList* list, StaticListNode* node, int pos);

StaticListNode* StaticList_Get(StaticList* list, int pos);

StaticListNode* StaticList_Delete(StaticList* list, int pos);

#endif//_STATIC_LIST_H_

/***********************************************************************************
TOP: 静态链表
  1.静态链表是柔性数组的一种典型用运
    struct Array
    {
        int capacity;
        int array[];
    };
  2.传递一个参数首先判断参数的合法性
  3.定义一个变量首先对它进行初始化 特别注意这些潜在的错误
  4.静态链表的首结点 保存了首地址next(即数组下表)和链表当前的长度(核心)

  *********************************************************************************/

#include "static_list.h"

#include <stdio.h>
#include <malloc.h>

#define AVAILABLE -1

typedef struct _tag_StaticNode
{
    unsigned int data;
    int next;
}TStaticNode;

typedef struct _tag_StaticList
{
    int capacity;
    TStaticNode header;
    TStaticNode node[];
}TStaticList;

StaticList* StaticList_Create(int capacity)/*O(1)*/
{
    TStaticList* ret = NULL;
    int i = 0;

    if( capacity >= 0)/*切记判断函数的参数是否合法*/
    {
        ret = (TStaticList*)malloc( sizeof(TStaticList) + sizeof(TStaticNode)*(capacity + 1) );
        /*多分配一个头结点用来保存首地址和当前长度 */
    }
    if( ret != NULL)
    {
        ret->capacity = capacity;
        ret->header.data = 0;
        ret->header.next = 0;

        for(i=1; i<= capacity; ++i)
        {
            ret->node[i].next = AVAILABLE;
        }
    }
    return ret;
}

void StaticList_Destroy(StaticList* list)/*O(1)*/
{
    free(list);
}
void StaticList_Clear(StaticList* list)/*O(1)*/
{
    TStaticList* sList = (TStaticList*)list;
    int i;

    if(sList != NULL)
    {
        sList->header.data = 0;/* 表示长度 第0个元素当头 */
        sList->header.next = 0;
    }

    for(i=1; i<= sList->capacity; ++i)
    {
        sList->node[i].next = AVAILABLE;
    }
}
int StaticList_Length(StaticList* list) // O(1)
{
    TStaticList* sList = (TStaticList*)list;
    int ret = -1;

    if( sList != NULL )
    {
        ret = sList->header.data;/* return length */
    }

    return ret;
}

int StaticList_Capacity(StaticList *list)
{
    TStaticList* sList = (TStaticList*)list;
    int ret = -1;

    if(sList != NULL)
    {
       ret = sList->capacity;/* return length */
    }

    return -1;
}
int StaticList_Insert(StaticList* list, StaticListNode* node, int pos)  // O(n)
{
    TStaticList* sList = (TStaticList*)list;
    int ret = (sList != NULL);
    int current = 0;
    int index = 0;
    int i = 0;

    ret = ret && (sList->header.data + 1 <= sList->capacity);
    ret = ret && (pos >=0) && (node != NULL);

    if( ret )
    {
        for(i=1; i<sList->capacity; i++) // 0保存了header 所以从1开始
        {
            if( sList->node[i].next == AVAILABLE )
            {
                index = i;
                break;
            }
        }

        sList->node[index].data = (unsigned int)node;

        sList->node[0] = sList->header;

        for(i=0; (i<pos) && (sList->node[current].next != 0); i++)
        {
            current = sList->node[current].next;
        }
        //end for current == pos
        sList->node[index].next = sList->node[current].next;
        sList->node[current].next = index;

        sList->node[0].data++;/* node[0].data 保存了当前的长度 */

        sList->header = sList->node[0];
    }

    return ret;
}

StaticListNode* StaticList_Get(StaticList* list, int pos)
{
    TStaticList* sList = (TStaticList*)list;
    StaticListNode* ret = NULL;
    int current = 0;
    int object = 0;
    int i;

    if( (sList != NULL)&&(0<= pos)&&(pos<=sList->header.data) )
    {
        sList->node[0] = sList->header;

        for(i=0; i<pos; i++)
        {
            current = sList->node[current].next;
        }
        object = sList->node[current].next;/* 要获取的元素的下表 */

        ret = (StaticListNode*)(sList->node[object].data);
    }

    return ret;
}
StaticListNode* StaticList_Delete(StaticList* list, int pos)
{
    TStaticList* sList = (TStaticList*)list;
    int ret = NULL;
    int current = 0;
    int object = 0;
    int i;

    if( (sList != NULL)&&(0<= pos)&&(pos<=sList->header.data) )
    {
        sList->node[0] = sList->header;

        for(i=0; i<pos; ++i)
        {
            current = sList->node[current].next;
        }
        object = sList->node[current].next;/* 要获取的元素的下表 */

        sList->node[current].next = sList->node[object].next;

        sList->node[0].data--;

        sList->node[object].next = AVAILABLE;

        sList->header = sList->node[0];

        ret = (StaticListNode*)(sList->node[object].data);
    }

    return ret;
}
//=====================================================================================================
#include "static_list.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    StaticList* list = StaticList_Create(10);

    int index = 0;

    int i = 0;
    int j = 1;
    int k = 2;
    int s = 3;
    int y = 4;

    StaticList_Insert(list, &i, 0);
    StaticList_Insert(list, &j, 0);
    StaticList_Insert(list, &k, 0);
    StaticList_Insert(list, &s, 0);
    StaticList_Insert(list, &y, 0);


    printf("length is %d\n", StaticList_Length(list));

    for(index=0; index<StaticList_Length(list); index++)
    {
        int *p = (int*)StaticList_Get(list, index);
        printf("%d\n", *p);
    }

    StaticList_Destroy(list);

    return 0;
}
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


#endif//_LINKQUEUE_H_

#include <malloc.h>
#include "linkqueue.h"

typedef struct _tag_LinkQueueNode
{
    struct _tag_LinkQueueNode* next;
    void* item;
}TLinkQueueNode;

typedef struct _tag_LinkQueue
{
    TLinkQueueNode* front;
    TLinkQueueNode* rear;
    int length;
}TLinkQueue;


LinkQueue* LinkQueue_Create()// O(1)
{
    TLinkQueue* ret = (TLinkQueue*)malloc(sizeof(TLinkQueue));

    if(ret != NULL)
    {
        ret->front = NULL;
        ret->rear = NULL;
        ret->length = 0;
    }

    return ret;
}

void LinkQueue_Destroy(LinkQueue* queue)// O(n)
{
    LinkQueue_Clear(queue);
    free(queue);
}

void LinkQueue_Clear(LinkQueue* queue)/* 由于我们在LinkQueue_Append()在#line:32中创建节点时分配了空间所以需要手工释放 */
{
    while( LinkQueue_Length(queue) > 0)
    {
        LinkQueue_Retrieve(queue);
    }
}

int LinkQueue_Append(LinkQueue* queue, void* item)// O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = (TLinkQueueNode*)malloc(sizeof(TLinkQueueNode));/* #line:32 */
    int ret = (item != NULL) && (node != NULL) && (sQueue != NULL);

    if(ret)
    {
        node->item = item;

        if(sQueue->length > 0)
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
    if(!ret)
    {
        free(node);
    }
    return ret;
}

void* LinkQueue_Retrieve(LinkQueue* queue)// O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    TLinkQueueNode* node = NULL;
    void* ret = NULL;

    if((sQueue != NULL) && (sQueue->length > 0))
    {
        node = sQueue->front; // node指向头结点

        sQueue->front = node->next;

        ret = node->item;

        free(node);

        sQueue->length--;

        if(0 == sQueue->length)
        {
            sQueue->front = NULL;
            sQueue->rear = NULL;
        }
    }
    return ret;
}

void* LinkQueue_Header(LinkQueue* queue)// O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    void* ret = NULL;

    if((sQueue != NULL) && (sQueue->length > 0))
    {
        ret = sQueue->front->item;
    }

    return ret;
}

int LinkQueue_Length(LinkQueue* queue)// O(1)
{
    TLinkQueue* sQueue = (TLinkQueue*)queue;
    int ret = -1;

    if(sQueue != NULL)
    {
        ret = sQueue->length;
    }

    return ret;
}

// =====================================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include "linkqueue.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[])
{
    LinkQueue* queue = LinkQueue_Create();
    int a[10] = {0};
    int i = 0;

    for(i=0; i<10; i++)
    {
        a[i] = i + 1;

        LinkQueue_Append(queue, a + i);
    }

    printf("Header: %d\n", *(int*)LinkQueue_Header(queue));
    printf("Length: %d\n", LinkQueue_Length(queue));


    while( LinkQueue_Length(queue) > 0 )
    {
        printf("Retrieve: %d\n", *(int*)LinkQueue_Retrieve(queue));
    }

    LinkQueue_Destroy(queue);

    return 0;
}
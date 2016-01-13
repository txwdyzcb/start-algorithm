#ifndef _SQEQUEUE_H_
#define _SQEQUEUE_H_

typedef void SeqQueue;

SeqQueue* SeqQueue_Create(int capacity);

void SeqQueue_Destroy(SeqQueue* queue);

void SeqQueue_Clear(SeqQueue* queue);

int SeqQueue_Append(SeqQueue* queue, void* item);

void* SeqQueue_Retrieve(SeqQueue* queue);

void* SeqQueue_Header(SeqQueue* queue);

int SeqQueue_Capacity(SeqQueue* queue);

int SeqQueue_Length(SeqQueue* queue);

#endif // _SQEQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "sqequeue.h"

typedef unsigned int TSeqQueueNode;

typedef struct _tag_SeqQueue
{
    int capacity;
    int length;
    int front;
    int rear;
    TSeqQueueNode* node;
}TSeqQueue;

SeqQueue* SeqQueue_Create(int capacity)// O(1)
{
    TSeqQueue* ret = NULL;
    if(capacity >= 0)
    {
        ret = (TSeqQueue*)malloc(sizeof(TSeqQueue) + sizeof(TSeqQueueNode) * capacity);
    }
    if(ret != NULL)
    {
        ret->capacity = capacity;
        ret->length = 0;
        ret->front = 0;
        ret->rear = 0;
        ret->node = (TSeqQueueNode*)(ret + 1);
    }
}

void SeqQueue_Destroy(SeqQueue* queue)// O(1)
{
    free(queue);
}

void SeqQueue_Clear(SeqQueue* queue)  // O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    if(sQueue != NULL)
    {
        sQueue->length = 0;
        sQueue->front = 0;
        sQueue->rear = 0;
    }
}

int SeqQueue_Append(SeqQueue* queue, void* item)// O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    int ret = (sQueue != NULL);

    ret = ret && (item != NULL) && (sQueue->length+1 <= sQueue->capacity);

    if(ret)
    {
        sQueue->node[sQueue->length] = (TSeqQueueNode*)item;
        sQueue->rear = (sQueue->rear + 1) % sQueue->capacity; // 为了提高空间的利用率 尾加了一个元素 则尾向后退一个单位 +1 
        sQueue->length++;
    }

    return ret;
}

void* SeqQueue_Retrieve(SeqQueue* queue)// O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    void* ret = SeqQueue_Header(queue);

    if(ret != NULL)
    {
        sQueue->front = (sQueue->front + 1) % sQueue->capacity; // 从头删除一个元素 则头向后退一个单位 +1

        sQueue->length--;
    }

    return ret;
}

void* SeqQueue_Header(SeqQueue* queue) // O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    void* ret = NULL;

    if((sQueue != NULL) && (sQueue->length > 0))
    {
        ret = (void*)(sQueue->node[sQueue->front]);
    }

    return ret;
}

int SeqQueue_Capacity(SeqQueue* queue) // O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    int ret = -1;

    if(sQueue != NULL)
    {
        ret = sQueue->capacity;
    }

    return ret;
}

int SeqQueue_Length(SeqQueue* queue) // O(1)
{
    TSeqQueue* sQueue = (TSeqQueue*)queue;
    int ret = -1;

    if(sQueue != NULL)
    {
        ret = sQueue->length;
    }

    return ret;
}

#include <stdio.h>
#include <stdlib.h>

#include "sqequeue.h"

int main(void)
{
    SeqQueue* queue = SeqQueue_Create(20);
    int a[10] = {0};
    int i = 0;
    for(i = 0; i < 10; ++i)
    {
        a[i] = i+1;
        SeqQueue_Append(queue, a + i);
    }
    printf("Header:  %d\n", *(int*)SeqQueue_Header(queue));
    printf("Length:  %d\n", SeqQueue_Length(queue));
    printf("Capacity:%d\n", SeqQueue_Capacity(queue));

    while( SeqQueue_Length(queue) > 0)
    {
        printf("Retrieve:%d\n", *(int*)SeqQueue_Retrieve(queue));
    }
    printf("Header:  %d\n", *(int*)SeqQueue_Header(queue));
    printf("Length:  %d\n", SeqQueue_Length(queue));
    printf("Capacity:%d\n", SeqQueue_Capacity(queue));


    SeqQueue_Destroy(queue);

    return 0;

}
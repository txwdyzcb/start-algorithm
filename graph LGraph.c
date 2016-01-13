#ifndef _LINKLIST_H_
#define _LINKLIST_H_

typedef void LinkList;
typedef struct _tag_LinkListNode LinkListNode;
struct _tag_LinkListNode
{
    LinkListNode* next;
};

LinkList* LinkList_Create();

void LinkList_Destroy(LinkList* list);

void LinkList_Clear(LinkList* list);

int LinkList_Length(LinkList* list);

int LinkList_Insert(LinkList* list, LinkListNode* node, int pos);

LinkListNode* LinkList_Get(LinkList* list, int pos);

LinkListNode* LinkList_Delete(LinkList* list, int pos);

#endif


#include <stdio.h>
#include <malloc.h>
#include "LinkList.h"

typedef struct _tag_LinkList
{
    LinkListNode header;
    int length;
} TLinkList;

LinkList* LinkList_Create() // O(1)
{
    TLinkList* ret = (TLinkList*)malloc(sizeof(TLinkList));
    
    if( ret != NULL )
    {
        ret->length = 0;
        ret->header.next = NULL;
    }
    
    return ret;
}

void LinkList_Destroy(LinkList* list) // O(1)
{
    free(list);
}

void LinkList_Clear(LinkList* list) // O(1)
{
    TLinkList* sList = (TLinkList*)list;
    
    if( sList != NULL )
    {
        sList->length = 0;
        sList->header.next = NULL;
    }
}

int LinkList_Length(LinkList* list) // O(1)
{
    TLinkList* sList = (TLinkList*)list;
    int ret = -1;
    
    if( sList != NULL )
    {
        ret = sList->length;
    }
    
    return ret;
}

int LinkList_Insert(LinkList* list, LinkListNode* node, int pos) // O(n)
{ 
    TLinkList* sList = (TLinkList*)list;
    int ret = (sList != NULL) && (pos >= 0) && (node != NULL);
    int i = 0;
    
    if( ret )
    {
        LinkListNode* current = (LinkListNode*)sList;
        
        for(i=0; (i<pos) && (current->next != NULL); i++)
        {
            current = current->next;
        }
        
        node->next = current->next;
        current->next = node;
        
        sList->length++;
    }
    
    return ret;
}

LinkListNode* LinkList_Get(LinkList* list, int pos) // O(n)
{
    TLinkList* sList = (TLinkList*)list;
    LinkListNode* ret = NULL;
    int i = 0;
    
    if( (sList != NULL) && (0 <= pos) && (pos < sList->length) )
    {
        LinkListNode* current = (LinkListNode*)sList;
        
        for(i=0; i<pos; i++)
        {
            current = current->next;
        }
        
        ret = current->next;
    }
    
    return ret;
}

LinkListNode* LinkList_Delete(LinkList* list, int pos) // O(n)
{
    TLinkList* sList = (TLinkList*)list;
    LinkListNode* ret = NULL;
    int i = 0;
    
    if( (sList != NULL) && (0 <= pos) && (pos < sList->length) )
    {
        LinkListNode* current = (LinkListNode*)sList;
        
        for(i=0; i<pos; i++)
        {
            current = current->next;
        }
        
        ret = current->next;
        current->next = ret->next;
        
        sList->length--;
    }
    
    return ret;
}


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


#ifndef _LGRAPH_H_
#define _LGRAPH_H_

typedef void LGraph;
typedef void LVertex;
typedef void (LGraph_Printf)(LVertex*);

LGraph* LGraph_Create(LVertex** v, int n);

void LGraph_Destroy(LGraph* graph);

void LGraph_Clear(LGraph* graph);

int LGraph_AddEdge(LGraph* graph, int v1, int v2, int w);

int LGraph_RemoveEdge(LGraph* graph, int v1, int v2);

int LGraph_GetEdge(LGraph* graph, int v1, int v2);

int LGraph_TD(LGraph* graph, int v);

int LGraph_VertexCount(LGraph* graph);

int LGraph_EdgeCount(LGraph* graph);

void LGraph_DFS(LGraph* graph, int v, LGraph_Printf* pFunc);

void LGraph_BFS(LGraph* graph, int v, LGraph_Printf* pFunc);

void LGraph_Display(LGraph* graph, LGraph_Printf* pFunc);

#endif


#include <malloc.h>
#include <stdio.h>
#include "LGraph.h"
#include "LinkList.h"
#include "LinkQueue.h"

typedef struct _tag_LGraph
{
    int count;
    LVertex** v;
    LinkList** la; // 这里是个链表组 实际上如果使用STL将数据结构抽象出来会更好理解 个人觉得
} TLGraph;

typedef struct _tag_ListNode
{
    LinkListNode header;
    int v; // 边另一端的顶点
    int w; // 权值
} TListNode;

LGraph* LGraph_Create(LVertex** v, int n)  // O(n)
{
    TLGraph* ret = NULL;
    int ok = 1;
    
    if( (v != NULL ) && (n > 0) ){    
        ret = (TLGraph*)malloc(sizeof(TLGraph));
        
        if( ret != NULL ) {
       
            ret->count = n;            
            ret->v = (LVertex**)calloc(n, sizeof(LVertex*));            
            ret->la = (LinkList**)calloc(n, sizeof(LinkList*));            
            ok = (ret->v != NULL) && (ret->la != NULL);
            
            if( ok ) {           
                int i = 0;                
                for(i=0; i<n; i++) {               
                    ret->v[i] = v[i];
                }
                
                for(i=0; (i<n) && ok; i++)  {              
                    ok = ok && ((ret->la[i] = LinkList_Create()) != NULL);
                }
            }

            if( !ok ) {           
                if( ret->la != NULL )  {              
                    int i = 0;                    
                    for(i=0; i<n; i++){                    
                        LinkList_Destroy(ret->la[i]);
                    }
                }
                
                free(ret->la);
                free(ret->v);
                free(ret);                
                ret = NULL;
            }
        }
    }    
    return ret;
}

void LGraph_Destroy(LGraph* graph) // O(n*n)
{
    TLGraph* tGraph = (TLGraph*)graph;    
    LGraph_Clear(tGraph);
    
    if( tGraph != NULL ){    
        int i = 0;        
        for(i=0; i<tGraph->count; i++){        
            LinkList_Destroy(tGraph->la[i]);
        }
        
        free(tGraph->la);
        free(tGraph->v);
        free(tGraph);
    }
}

void LGraph_Clear(LGraph* graph) // O(n*n)
{
    TLGraph* tGraph = (TLGraph*)graph;
    
    if( tGraph != NULL ) {   
        int i = 0;        
        for(i=0; i<tGraph->count; i++){        
            while( LinkList_Length(tGraph->la[i]) > 0 ) {           
                free(LinkList_Delete(tGraph->la[i], 0));
            }
        }
    }
}

int LGraph_AddEdge(LGraph* graph, int v1, int v2, int w) // O(1) 添加边构造图
{
    TLGraph* tGraph = (TLGraph*)graph;
    TListNode* node = NULL;
    int ret = (tGraph != NULL);
    
    ret = ret && (0 <= v1) && (v1 < tGraph->count);
    ret = ret && (0 <= v2) && (v2 < tGraph->count);
    ret = ret && (0 < w) && ((node = (TListNode*)malloc(sizeof(TListNode))) != NULL);
    
    if( ret ){    
       node->v = v2;
       node->w = w;
       
       LinkList_Insert(tGraph->la[v1], (LinkListNode*)node, 0);
    }
    
    return ret;
}

int LGraph_RemoveEdge(LGraph* graph, int v1, int v2) // O(n*n)
{
    TLGraph* tGraph = (TLGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v1) && (v1 < tGraph->count);
    condition = condition && (0 <= v2) && (v2 < tGraph->count);
    
    if( condition ) {   
        TListNode* node = NULL;
        int i = 0;
        
        for(i=0; i<LinkList_Length(tGraph->la[v1]); i++){        
            node = (TListNode*)LinkList_Get(tGraph->la[v1], i);            
            if( node->v == v2){            
                ret = node->w;

                LinkList_Delete(tGraph->la[v1], i);                
                free(node);                
                break;
            }
        }
    }    
    return ret;
}

int LGraph_GetEdge(LGraph* graph, int v1, int v2) // O(n*n)
{
    TLGraph* tGraph = (TLGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v1) && (v1 < tGraph->count);
    condition = condition && (0 <= v2) && (v2 < tGraph->count);
    
    if( condition ) {   
        TListNode* node = NULL;
        int i = 0;
        
        for(i=0; i<LinkList_Length(tGraph->la[v1]); i++) {       
            node = (TListNode*)LinkList_Get(tGraph->la[v1], i);
            
            if( node->v == v2) {           
                ret = node->w;                
                break;
            }
        }
    }    
    return ret;
}

int LGraph_TD(LGraph* graph, int v) // O(n*n*n) 这里是双向的度数
{
    TLGraph* tGraph = (TLGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    
    if( condition ){    
        int i = 0;
        int j = 0;        
        for(i=0; i<tGraph->count; i++) {       
            for(j=0; j<LinkList_Length(tGraph->la[i]); j++) {          
                TListNode* node = (TListNode*)LinkList_Get(tGraph->la[i], j);                
                if( node->v == v ) {              
                    ret++;
                }
            }
        }        
        ret += LinkList_Length(tGraph->la[v]);
    }    
    return ret;
}

int LGraph_VertexCount(LGraph* graph) // O(1)
{
    TLGraph* tGraph = (TLGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ) {   
        ret = tGraph->count;
    }    
    return ret;
}

int LGraph_EdgeCount(LGraph* graph) // O(n) 边的数量实际上就是每个链表的长度
{
    TLGraph* tGraph = (TLGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ){    
        int i = 0;        
        for(i=0; i<tGraph->count; i++){        
            ret += LinkList_Length(tGraph->la[i]);
        }
    }
    
    return ret;
}

static void recursive_dfs(TLGraph* graph, int v, int visited[], LGraph_Printf* pFunc)
{
    int i = 0;
    
    pFunc(graph->v[v]);
    
    visited[v] = 1;
    
    printf(", ");
    
    for(i=0; i<LinkList_Length(graph->la[v]); i++)
    {
        TListNode* node = (TListNode*)LinkList_Get(graph->la[v], i);
        
        if( !visited[node->v] )
        {
            recursive_dfs(graph, node->v, visited, pFunc);
        }
    }
}

static void bfs(TLGraph* graph, int v, int visited[], LGraph_Printf* pFunc)
{
    LinkQueue* queue = LinkQueue_Create();
    
    if( queue != NULL )
    {
        LinkQueue_Append(queue, graph->v + v);
        
        visited[v] = 1;
        
        while( LinkQueue_Length(queue) > 0 )
        {
            int i = 0;
            
            v = (LVertex**)LinkQueue_Retrieve(queue) - graph->v;
            
            pFunc(graph->v[v]);
            
            printf(", ");
            
            for(i=0; i<LinkList_Length(graph->la[v]); i++)
            {
                TListNode* node = (TListNode*)LinkList_Get(graph->la[v], i);
                
                if( !visited[node->v] )
                {
                    LinkQueue_Append(queue, graph->v + node->v);
                    
                    visited[node->v] = 1;
                }
            }
        }
    }
    
    LinkQueue_Destroy(queue);
}
void LGraph_DFS(LGraph* graph, int v, LGraph_Printf* pFunc)
{
    TLGraph* tGraph = (TLGraph*)graph;
    int* visited = NULL;
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition )
    {
        int i = 0;
        
        recursive_dfs(tGraph, v, visited, pFunc);
        
        for(i=0; i<tGraph->count; i++)
        {
            if( !visited[i] )
            {
                recursive_dfs(tGraph, i, visited, pFunc);
            }
        }
        
        printf("\n");
    }
    
    free(visited);
}

void LGraph_BFS(LGraph* graph, int v, LGraph_Printf* pFunc)
{
    TLGraph* tGraph = (TLGraph*)graph;
    int* visited = NULL;
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition )
    {
        int i = 0;
        
        bfs(tGraph, v, visited, pFunc);
        
        for(i=0; i<tGraph->count; i++)
        {
            if( !visited[i] )
            {
                bfs(tGraph, i, visited, pFunc);
            }
        }
        
        printf("\n");
    }
    
    free(visited);
}

void LGraph_Display(LGraph* graph, LGraph_Printf* pFunc) // O(n*n*n)
{
    TLGraph* tGraph = (TLGraph*)graph;
    
    if( (tGraph != NULL) && (pFunc != NULL) )
    {
        int i = 0;
        int j = 0;
        
        for(i=0; i<tGraph->count; i++)
        {
            printf("%d:", i);
            pFunc(tGraph->v[i]);
            printf(" ");
        }
        
        printf("\n");
        
        for(i=0; i<tGraph->count; i++)
        {
            for(j=0; j<LinkList_Length(tGraph->la[i]); j++)
            {
                TListNode* node = (TListNode*)LinkList_Get(tGraph->la[i], j);
                
                printf("<");
                pFunc(tGraph->v[i]);
                printf(", ");
                pFunc(tGraph->v[node->v]);
                printf(", %d", node->w);
                printf(">");
                printf(" ");               
            }
        }
        
        printf("\n");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "LGraph.h"

void print_data(LVertex* v)
{
    printf("%s", (char*)v);
}

int main(int argc, char *argv[])
{
    LVertex* v[] = {"A", "B", "C", "D", "E", "F"};
    LGraph* graph = LGraph_Create(v, 6);
    
    LGraph_AddEdge(graph, 0, 1, 1);
    LGraph_AddEdge(graph, 0, 2, 1);
    LGraph_AddEdge(graph, 0, 3, 1);
    LGraph_AddEdge(graph, 1, 5, 1);
    LGraph_AddEdge(graph, 1, 4, 1);
    LGraph_AddEdge(graph, 2, 1, 1);
    LGraph_AddEdge(graph, 3, 4, 1);
    LGraph_AddEdge(graph, 4, 2, 1);
    
    LGraph_Display(graph, print_data);
    
    LGraph_DFS(graph, 0, print_data);
    LGraph_BFS(graph, 0, print_data);
    
    LGraph_Destroy(graph);
    
    return 0;
}


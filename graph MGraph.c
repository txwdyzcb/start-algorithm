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



#ifndef _MGRAPH_H_
#define _MGRAPH_H_

typedef void MGraph;
typedef void MVertex;
typedef void (MGraph_Printf)(MVertex*);

MGraph* MGraph_Create(MVertex** v, int n);

void MGraph_Destroy(MGraph* graph);

void MGraph_Clear(MGraph* graph);

int MGraph_AddEdge(MGraph* graph, int v1, int v2, int w);

int MGraph_RemoveEdge(MGraph* graph, int v1, int v2);

int MGraph_GetEdge(MGraph* graph, int v1, int v2);

int MGraph_TD(MGraph* graph, int v);

int MGraph_VertexCount(MGraph* graph);

int MGraph_EdgeCount(MGraph* graph);

void MGraph_DFS(MGraph* graph, int v, MGraph_Printf* pFunc);

void MGraph_BFS(MGraph* graph, int v, MGraph_Printf* pFunc);

void MGraph_Display(MGraph* graph, MGraph_Printf* pFunc);

#endif



#include <malloc.h>
#include <stdio.h>
#include "MGraph.h"
#include "LinkQueue.h"

typedef struct _tag_MGraph
{
    int count;
    MVertex** v;  // 定点v
    int** matrix; // 用来保存边
} TMGraph;

MGraph* MGraph_Create(MVertex** v, int n)  // O(n)
{
    TMGraph* ret = NULL;
    
    if( (v != NULL ) && (n > 0) ){    
        ret = (TMGraph*)malloc(sizeof(TMGraph));
        
        if( ret != NULL ){        
            int* p = NULL;            
            ret->count = n;            
            ret->v = (MVertex**)malloc(sizeof(MVertex*) * n);            
            ret->matrix = (int**)malloc(sizeof(int*) * n);            
            p = (int*)calloc(n * n, sizeof(int));     

            if( (ret->v != NULL) && (ret->matrix != NULL) && (p != NULL) ){            
                int i = 0;                
                for(i=0; i<n; i++) {               
                    ret->v[i] = v[i];
                    ret->matrix[i] = p + i * n;
                }
            }
            else{            
                free(p);
                free(ret->matrix);
                free(ret->v);
                free(ret);                
                ret = NULL;
            }
        }
    }
    
    return ret;
}

void MGraph_Destroy(MGraph* graph) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
    if( tGraph != NULL ) {   
        free(tGraph->v);
        free(tGraph->matrix[0]);
        free(tGraph->matrix);
        free(tGraph);
    }
}

void MGraph_Clear(MGraph* graph) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
    if( tGraph != NULL )
    {
        int i = 0;
        int j = 0;        
        for(i=0; i<tGraph->count; i++){        
            for(j=0; j<tGraph->count; j++){            
                tGraph->matrix[i][j] = 0;
            }
        }
    }
}

int MGraph_AddEdge(MGraph* graph, int v1, int v2, int w) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = (tGraph != NULL);
    
    ret = ret && (0 <= v1) && (v1 < tGraph->count);
    ret = ret && (0 <= v2) && (v2 < tGraph->count);
    ret = ret && (0 <= w);
    
    if( ret ){    
        tGraph->matrix[v1][v2] = w;
    }    
    return ret;
}

int MGraph_RemoveEdge(MGraph* graph, int v1, int v2) // O(1)
{
    int ret = MGraph_GetEdge(graph, v1, v2);
    
    if( ret != 0 ){    
        ((TMGraph*)graph)->matrix[v1][v2] = 0;
    }
    
    return ret;
}

int MGraph_GetEdge(MGraph* graph, int v1, int v2) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v1) && (v1 < tGraph->count);
    condition = condition && (0 <= v2) && (v2 < tGraph->count);
    
    if( condition ){    
        ret = tGraph->matrix[v1][v2];
    }    
    return ret;
}

int MGraph_TD(MGraph* graph, int v) // O(n) 返回于定点v相关的边数即顶点v的度 这是有向图 无向图的话会重复
{
    TMGraph* tGraph = (TMGraph*)graph;
    int condition = (tGraph != NULL);
    int ret = 0;
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    
    if( condition ) {   
        int i = 0;        
        for(i=0; i<tGraph->count; i++){        
            if( tGraph->matrix[v][i] != 0 ) {           
                ret++;
            }
            
            if( tGraph->matrix[i][v] != 0 ){            
                ret++;
            }
        }
    }    
    return ret;
}

int MGraph_VertexCount(MGraph* graph) // O(1)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ) {   
        ret = tGraph->count;
    }    
    return ret;
}

int MGraph_EdgeCount(MGraph* graph) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int ret = 0;
    
    if( tGraph != NULL ) {   
        int i = 0;
        int j = 0;        
        for(i=0; i<tGraph->count; i++){        
            for(j=0; j<tGraph->count; j++) {           
                if( tGraph->matrix[i][j] != 0 ){                
                    ret++;
                }
            }
        }
    }    
    return ret;
}

static void recursive_dfs(TMGraph* graph, int v, int visited[], MGraph_Printf* pFunc)
{
    int i = 0;    
    pFunc(graph->v[v]);    
    visited[v] = 1;    
    printf(", ");
    
    for(i=0; i<graph->count; i++){    
        if( (graph->matrix[v][i] != 0) && !visited[i] ){ // 深度优先遍历 直接遍历某个节点边对应的另外一个端点        
            recursive_dfs(graph, i, visited, pFunc);
        }
    }
}

static void bfs(TMGraph* graph, int v, int visited[], MGraph_Printf* pFunc)
{
    LinkQueue* queue = LinkQueue_Create();
    
    if( queue != NULL ){
        LinkQueue_Append(queue, graph->v + v); // 直接+v 跳到v对应的地方 实际上就是 v[v][0]       
        visited[v] = 1; 
        
        while( LinkQueue_Length(queue) > 0 ){
            int i = 0;            
            v = (MVertex**)LinkQueue_Retrieve(queue) - graph->v; // 出队列减去 graph->v 起始地址得到偏移量            
            pFunc(graph->v[v]);            
            printf(", ");
            
            for(i=0; i<graph->count; i++){
                if( (graph->matrix[v][i] != 0) && !visited[i] ){
                    LinkQueue_Append(queue, graph->v + i); // 压人的时候 实际上是压入了定点v  这里的遍历可以理解成一颗大树                
                    visited[i] = 1;
                }
            }
        }
    }    
    LinkQueue_Destroy(queue);
}

void MGraph_DFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int* visited = NULL; // visited[]数组记录了访问过的节点 便于回退
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition ){                
        recursive_dfs(tGraph, v, visited, pFunc);     

        int i = 0;      
        for(i=0; i<tGraph->count; i++){ // 如果有某些节点没有遍历过 继续遍历       
            if( !visited[i] ){            
                recursive_dfs(tGraph, i, visited, pFunc);
            }
        }        
        printf("\n");
    }    
    free(visited);
}

void MGraph_BFS(MGraph* graph, int v, MGraph_Printf* pFunc)
{
    TMGraph* tGraph = (TMGraph*)graph;
    int* visited = NULL;
    int condition = (tGraph != NULL);
    
    condition = condition && (0 <= v) && (v < tGraph->count);
    condition = condition && (pFunc != NULL);
    condition = condition && ((visited = (int*)calloc(tGraph->count, sizeof(int))) != NULL);
    
    if( condition ) {    
        bfs(tGraph, v, visited, pFunc);
        int i = 0;
        for(i=0; i<tGraph->count; i++){        
            if( !visited[i] ){            
                bfs(tGraph, i, visited, pFunc);
            }
        }        
        printf("\n");
    }    
    free(visited);
}

void MGraph_Display(MGraph* graph, MGraph_Printf* pFunc) // O(n*n)
{
    TMGraph* tGraph = (TMGraph*)graph;
    
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
            for(j=0; j<tGraph->count; j++)
            {
                if( tGraph->matrix[i][j] != 0 )
                {
                    printf("<");
                    pFunc(tGraph->v[i]);
                    printf(", ");
                    pFunc(tGraph->v[j]);
                    printf(", %d", tGraph->matrix[i][j]);
                    printf(">");
                    printf(" ");
                }
            }
        }        
        printf("\n");
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "MGraph.h"

void print_data(MVertex* v){
    printf("%s", (char*)v);
}

int main(int argc, char *argv[])
{
    MVertex* v[] = {"A", "B", "C", "D", "E", "F"};
    MGraph* graph = MGraph_Create(v, 6);
    
    MGraph_AddEdge(graph, 0, 1, 1);
    MGraph_AddEdge(graph, 0, 2, 1);
    MGraph_AddEdge(graph, 0, 3, 1);
    MGraph_AddEdge(graph, 1, 5, 1);
    MGraph_AddEdge(graph, 1, 4, 1);
    MGraph_AddEdge(graph, 2, 1, 1);
    MGraph_AddEdge(graph, 3, 4, 1);
    MGraph_AddEdge(graph, 4, 2, 1);
    
    MGraph_Display(graph, print_data);
    
    MGraph_DFS(graph, 0, print_data);
    MGraph_BFS(graph, 0, print_data);
    
    MGraph_Destroy(graph);
    
    return 0;
}
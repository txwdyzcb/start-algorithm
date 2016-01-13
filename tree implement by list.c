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


#ifndef _GTREE_H_
#define _GTREE_H_

typedef void GTree;
typedef void GTreeData;
typedef void (GTree_Printf)(GTreeData*);

GTree* GTree_Create();

void GTree_Destroy(GTree* tree);

void GTree_Clear(GTree* tree);

int GTree_Insert(GTree* tree, GTreeData* data, int pPos);

GTreeData* GTree_Delete(GTree* tree, int pos);

GTreeData* GTree_Get(GTree* tree, int pos);

GTreeData* GTree_Root(GTree* tree);

int GTree_Height(GTree* tree);

int GTree_Count(GTree* tree);

int GTree_Degree(GTree* tree);

void GTree_Display(GTree* tree, GTree_Printf* pFunc, int gap, char div);

#endif


#include <stdio.h>
#include <malloc.h>
#include "GTree.h"
#include "LinkList.h"


typedef struct _tag_GTreeNode GTreeNode;
struct _tag_GTreeNode
{
    GTreeData* data;
    GTreeNode* parent;
    LinkList* child;
};


typedef struct _tag_TLNode TLNode;
struct _tag_TLNode
{
    LinkListNode header;
    GTreeNode* node;
};


static void recursive_display(GTreeNode* node, GTree_Printf* pFunc, int format, int gap, char div)
{
    int i = 0;
    
    if( (node != NULL) && (pFunc != NULL) )
    {
        for(i=0; i<format; i++)
        {
            printf("%c", div);
        }
    
        pFunc(node->data);
    
        printf("\n");
    
        for(i=0; i<LinkList_Length(node->child); i++)
        {
            TLNode* trNode = (TLNode*)LinkList_Get(node->child, i);
            
            recursive_display(trNode->node, pFunc, format + gap, gap, div);
        }
    }
}

static void recursive_delete(LinkList* list, GTreeNode* node)
{
    if( (list != NULL) && (node != NULL) )
    {
        GTreeNode* parent = node->parent;
        int index = -1;
        int i = 0;
        
        for(i=0; i<LinkList_Length(list); i++)
        {
            TLNode* trNode = (TLNode*)LinkList_Get(list, i);
             
            if( trNode->node == node )
            {
                LinkList_Delete(list, i);
                
                free(trNode);
                
                index = i;
                
                break;
            }
        }
          
        if( index >= 0 )
        {  
            if( parent != NULL )
            {
                 for(i=0; i<LinkList_Length(parent->child); i++)
                 {
                     TLNode* trNode = (TLNode*)LinkList_Get(parent->child, i);
                     
                     if( trNode->node == node )
                     {
                         LinkList_Delete(parent->child, i);
                         
                         free(trNode);
                         
                         break;
                     }
                 }               
            }
            
            while( LinkList_Length(node->child) > 0 )
            {
                TLNode* trNode = (TLNode*)LinkList_Get(node->child, 0);
                
                recursive_delete(list, trNode->node);
            }
            
            LinkList_Destroy(node->child);
        
            free(node);
        }
    }
}

static int recursive_height(GTreeNode* node)
{
    int ret = 0;
    
    if( node != NULL )
    {
        int subHeight = 0;
        int i = 0;
        
        for(i=0; i<LinkList_Length(node->child); i++)
        {
            TLNode* trNode = (TLNode*)LinkList_Get(node->child, i);
            
            subHeight = recursive_height(trNode->node);
            
            if( ret < subHeight )
            {
                ret = subHeight;
            }
        }
        
        ret = ret + 1;
    }
    
    return ret;
}

static int recursive_degree(GTreeNode* node)
{
int ret = -1;
    
    if( node != NULL )
    {
        int subDegree = 0;
        int i = 0;
        
        ret = LinkList_Length(node->child);
        
        for(i=0; i<LinkList_Length(node->child); i++)
        {
            TLNode* trNode = (TLNode*)LinkList_Get(node->child, i);
            
            subDegree = recursive_degree(trNode->node);
            
            if( ret < subDegree )
            {
                ret = subDegree;
            }
        }
    }
    
    return ret;
}

GTree* GTree_Create()
{
    return LinkList_Create();
}

void GTree_Destroy(GTree* tree)
{
    GTree_Clear(tree);
    LinkList_Destroy(tree);
}

void GTree_Clear(GTree* tree)
{
     GTree_Delete(tree, 0);
}

int GTree_Insert(GTree* tree, GTreeData* data, int pPos)
{
    LinkList* list = (LinkList*)tree;
    int ret = (list != NULL) && (data != NULL) && (pPos < LinkList_Length(list));
    
    if( ret )
    {
        TLNode* trNode = (TLNode*)malloc(sizeof(TLNode));
        TLNode* cldNode = (TLNode*)malloc(sizeof(TLNode));
        TLNode* pNode = (TLNode*)LinkList_Get(list, pPos);
        GTreeNode* cNode = (GTreeNode*)malloc(sizeof(GTreeNode));
        
        ret = (trNode != NULL) && (cldNode != NULL) && (cNode != NULL);
        
        if( ret )
        {
            cNode->data = data;
            cNode->parent = NULL;
            cNode->child = LinkList_Create();
            
            trNode->node = cNode;
            cldNode->node = cNode;
            
            LinkList_Insert(list, (LinkListNode*)trNode, LinkList_Length(list));
            
            if( pNode != NULL )
            {
                cNode->parent = pNode->node;
                
                LinkList_Insert(pNode->node->child, (LinkListNode*)cldNode, LinkList_Length(pNode->node->child));
            }
        }
        else
        {
            free(trNode);
            free(cldNode);
            free(cNode);
        }
    }
    
    return ret;
}

GTreeData* GTree_Delete(GTree* tree, int pos)
{
    TLNode* trNode = (TLNode*)LinkList_Get(tree, pos);
    GTreeData* ret = NULL;
    
    if( trNode != NULL )
    {
        ret = trNode->node->data;
        
        recursive_delete(tree, trNode->node);
    }
    
    return ret;
}

GTreeData* GTree_Get(GTree* tree, int pos)
{
    TLNode* trNode = (TLNode*)LinkList_Get(tree, pos);
    GTreeData* ret = NULL;
    
    if( trNode != NULL )
    {
        ret = trNode->node->data;
    }
    
    return ret;
}

GTreeData* GTree_Root(GTree* tree)
{
    return GTree_Get(tree, 0);
}

int GTree_Height(GTree* tree)
{
    TLNode* trNode = (TLNode*)LinkList_Get(tree, 0);
    int ret = 0;
    
    if( trNode != NULL )
    {
        ret = recursive_height(trNode->node);
    }
    
    return ret;
}

int GTree_Count(GTree* tree)
{
    return LinkList_Length(tree);
}

int GTree_Degree(GTree* tree)
{
    TLNode* trNode = (TLNode*)LinkList_Get(tree, 0);
    int ret = -1;
    
    if( trNode != NULL )
    {
        ret = recursive_degree(trNode->node);
    }
    
    return ret;
}

void GTree_Display(GTree* tree, GTree_Printf* pFunc, int gap, char div)
{
    TLNode* trNode = (TLNode*)LinkList_Get(tree, 0);
    
    if( (trNode != NULL) && (pFunc != NULL) )
    {  
        recursive_display(trNode->node, pFunc, 0, gap, div);
    }
}

#include <stdio.h>
#include "GTree.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void printf_data(GTreeData* data)
{
    printf("%c", (int)data);
}

int main(int argc, char *argv[])
{
    GTree* tree = GTree_Create();
    int i = 0;
    
    GTree_Insert(tree, (GTreeData*)'A', -1);
    GTree_Insert(tree, (GTreeData*)'B', 0);
    GTree_Insert(tree, (GTreeData*)'C', 0);
    GTree_Insert(tree, (GTreeData*)'D', 0);
    GTree_Insert(tree, (GTreeData*)'E', 1);
    GTree_Insert(tree, (GTreeData*)'F', 1);
    GTree_Insert(tree, (GTreeData*)'H', 3);
    GTree_Insert(tree, (GTreeData*)'I', 3);
    GTree_Insert(tree, (GTreeData*)'J', 3);
    
    printf("Tree Height: %d\n", GTree_Height(tree));
    printf("Tree Degree: %d\n", GTree_Degree(tree));
    printf("Full Tree:\n");
    
    GTree_Display(tree, printf_data, 2, ' ');
    
    printf("Get Tree Data:\n");
    
    for(i=0; i<GTree_Count(tree); i++)
    {
        printf_data(GTree_Get(tree, i));
        printf("\n");
    }
    
    printf("Get Root Data:\n");
    
    printf_data(GTree_Root(tree));
    
    printf("\n");
    
    GTree_Delete(tree, 3);
     
    printf("After Deleting D:\n");
    
    GTree_Display(tree, printf_data, 2, '-');
    
    GTree_Clear(tree);
    
    printf("After Clearing Tree:\n");
    
    GTree_Display(tree, printf_data, 2, '.');
        
    GTree_Destroy(tree);
    
    return 0;
}

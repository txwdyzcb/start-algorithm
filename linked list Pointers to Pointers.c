#ifndef _LIST_H_
#define _LIST_H_

#include <assert.h>

typedef struct node
{
    int data;
    struct node* next;
} node_t;

typedef void (*FUNC)(node_t*);
typedef int (*FUNC2)(node_t*);

void list_insert_front(node_t** head, int data);
void list_for_each(node_t* head, FUNC f);
void list_free(node_t** head);

node_t* list_get_node(node_t* head, int index);
void list_insert_at(node_t** head, int data, int index);
void list_remove_at(node_t** head, int index);
node_t* list_find(node_t* head, int data, int* ret);

//node_t* list_remove_if(node_t* head, FUNC2 f);
void list_remove_if(node_t** head, FUNC2 f);

#endif /* _LIST_H_ */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

void list_insert_front(node_t** head, int data) // 头节点可能改变所以需要传入二级指针
{
    node_t* n = (node_t*)malloc(sizeof(node_t));
    assert(n != NULL);
    n->data = data;
    n->next = NULL;

    if (*head == NULL)
        *head = n;
    else
    {
        n->next = *head;
        *head = n;
    }
}

void list_for_each(node_t* head, FUNC f)
{
    while (head)
    {
        f(head);
        head = head->next;
    }
}

void list_free(node_t** head)
{
    node_t* tmp;
    while (*head)
    {
        tmp = *head;
        *head = (*head)->next;
        free(tmp);
    }
}

node_t* list_get_node(node_t* head, int index)
{
    assert(index >= 0);

    int j = 0;
    while (head && j < index)
    {
        head = head->next;
        j++;
    }

    return head;
}

void list_insert_at(node_t** head, int data, int index)
{
    assert(index >= 0);

    if (index == 0)
        return list_insert_front(head, data);

    node_t* p;
    p = list_get_node(*head, index - 1);
    if (p == NULL)
    {
        fprintf(stderr, "error insert pos\n");
        exit(EXIT_FAILURE);
    }

    node_t* n = (node_t*)malloc(sizeof(node_t));
    assert(n != NULL);
    n->data = data;
    n->next = NULL;

    n->next = p->next;
    p->next = n;
}

void list_remove_at(node_t** head, int index)
{
    assert(index >= 0);
    node_t* n;
    if (index == 0)
    {
        n = *head;
        free(n);
        *head = (*head)->next;
    }
    else
    {
        node_t* p = list_get_node(*head, index - 1);
        if (p == NULL || p->next == NULL)
        {
            fprintf(stderr, "error remove pos\n");
            exit(EXIT_FAILURE);
        }
        n = p->next;
        p->next = n->next;
        free(n);
    }
}

node_t* list_find(node_t* head, int data, int* ret)
{
    node_t *ret = -1;
    int i = 0;
    while (head)
    {
        if (head->data == data)
        {
            *ret = i;
            break;
        }
        head = head->next;
        i++;
    }

    return head;
}

void list_remove_if(node_t** head, FUNC2 f)
{
    node_t** curr = head;
    while (*curr)
    {
        node_t* entry = *curr;
        if (f(entry))
        {
            *curr = entry->next;
            free(entry);
        }
        else
            curr = &entry->next;
    }
}

// ===========================================================================================================

#include "list.h"
#include <stdio.h>

void print_node(node_t* n)
{
    printf("data=%d ", n->data);
}

int greater_than_10(node_t* n)
{
    return n->data > 10;
}

int main(void)
{
    node_t* head = NULL;
    list_insert_front(&head, 30);
    list_insert_front(&head, 20);
    list_insert_front(&head, 10);

    list_for_each(head, print_node);
    putchar('\n');


    node_t* n = NULL;
    n = list_get_node(head, 1);
    if (n != NULL)
        printf("data = %d\n", n->data);
    else
        printf("not found\n");

    list_insert_at(&head, 15, 1);
    list_for_each(head, print_node);
    putchar('\n');

    list_remove_at(&head, 1);
    list_for_each(head, print_node);
    putchar('\n');

    int ret;
    n = list_find(head, 20, &ret);
    if (n != NULL)
        printf("data = %d index = %d\n", n->data, ret);
    else
        printf("not found\n");

    list_remove_if(&head, greater_than_10);
    list_for_each(head, print_node);
    putchar('\n');

    list_free(&head);
    assert(head == NULL);

    return 0;
}
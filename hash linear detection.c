#ifndef _HASHENTRY_H_
#define _HASHENTRY_H_

typedef struct node
{
    void* key;
    void* value;
}node_t;

typedef void (*ForeachFunc)(void*);
typedef int (*HashFunc)(int, void*);
typedef struct hash
{
    node_t* nodes;
    int bucketsSize;
    HashFunc hashFunc;
}hash_t;

hash_t* Hash_alloc(int bucketsSize, HashFunc hashFunc);

void* Hash_find_entry(hash_t* hash, void* key, int keySize);

void Hash_add_entry(hash_t* hash, void* key, int keySize, void* value, int valueSize);

void Hash_free_entry(hash_t* hash, void* key, int keySize);

void Hash_destroy(hash_t* hash);

void Hash_foreach(hash_t* hash, ForeachFunc KeyForeachFunc, ForeachFunc ValueForeachFunc);

#endif // _HASHENTRY_H_

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "hashentry.h"

hash_t* Hash_alloc(int bucketsSize, HashFunc hashFunc)
{
    hash_t* hash = (hash_t*)malloc(sizeof(hash_t) + bucketsSize*sizeof(node_t)); // 头 + node_t数组
    assert(hash);

    hash->bucketsSize = bucketsSize;
    hash->hashFunc = hashFunc;
    hash->nodes = (node_t*)(hash + 1); // 数组的每个元素为一个node_t*即node指针

    int i = 0;
    for(i = 0; i < hash->bucketsSize; ++i)
    {
        hash->nodes[i].key = NULL;
        hash->nodes[i].value = NULL;
    }
    return hash;
}

void* Hash_find_entry(hash_t* hash, void* key, int keySize)
{
    assert(hash && key);

    int bucket = (*hash->hashFunc)(hash->bucketsSize, key);  // 得到key对应的起始 bucket 开始遍历 做对比
    assert(0 <= bucket && bucket < hash->bucketsSize);

    int curr = bucket;
    while(hash->nodes[curr].key)
    {
        if(0 == memcmp(hash->nodes[curr].key, key, keySize))
        {
            return hash->nodes[curr].value;
        }

        curr = (curr+1) % hash->bucketsSize;                  // 这里相当于从curr查找空的位置 循环查找一圈
        if(curr == bucket)                                    // 循环了一圈 发现 没有空位置
            return NULL;
    }
    return NULL;
}

// 如果当前位置有元素的话 插入下一个位置
void Hash_add_entry(hash_t* hash, void* key, int keySize, void* value, int valueSize)
{
    assert(hash && key && value);

    if(Hash_find_entry(hash, key, keySize)) // value已经存在了
        return;

    int bucket = (*hash->hashFunc)(hash->bucketsSize, key);
    assert(0 <= bucket && bucket < hash->bucketsSize);

    int curr = bucket;
    while(hash->nodes[curr].key)
    {
        curr = (curr+1) % hash->bucketsSize; // 这里相当于从curr查找空的位置 循环查找一圈
        if(curr == bucket)                   // 循环了一圈 发现 没有空位置
            return;
    }

    hash->nodes[curr].key = malloc(keySize);
    hash->nodes[curr].value = malloc(valueSize);

    assert(hash->nodes[curr].key && hash->nodes[curr].value);

    memcpy(hash->nodes[curr].key, key, keySize);   // 这里加size是因为可能是char[] 字符数组等
    memcpy(hash->nodes[curr].value, value, valueSize);
}

void Hash_free_entry(hash_t* hash, void* key, int keySize)
{
    assert(hash && key);

    int bucket = (*hash->hashFunc)(hash->bucketsSize, key);
    assert(0 <= bucket && bucket < hash->bucketsSize);

    int curr = bucket;
    while(hash->nodes[curr].key)
    {
        if(0 == memcmp(hash->nodes[curr].key, key, keySize)) // 注意这里要用memcmp函数
        {
            free(hash->nodes[curr].key);
            free(hash->nodes[curr].value);
            hash->nodes[curr].key = NULL;
            hash->nodes[curr].value = NULL;
            return;
        }
        curr = (curr+1) % hash->bucketsSize; // 这里相当于从curr查找空的位置 循环查找一圈
        if(curr == bucket)                   // 循环了一圈 发现 没有空位置
            return;
    }
}

void Hash_destroy(hash_t* hash)
{
    int i = 0;
    for(i = 0; i < hash->bucketsSize; ++i)  // 1.释放数组里 保存的地址指针 对应的空间
    {
        if(hash->nodes[i].key)
        {
            free(hash->nodes[i].key);
            free(hash->nodes[i].value);
            hash->nodes[i].key = NULL;
            hash->nodes[i].value = NULL;
        }
    }
    free(hash);                           // 释放头和数组空间
}
void Hash_foreach(hash_t* hash, ForeachFunc KeyForeachFunc, ForeachFunc ValueForeachFunc)
{
    //assert(hash && KeyForeachFunc && ValueForeachFunc);

    int i = 0;
    for(i = 0; i < hash->bucketsSize; ++i)
    {
        if(hash->nodes[i].key)
        {
            KeyForeachFunc(hash->nodes[i].key);
            ValueForeachFunc(hash->nodes[i].value);
        }
    }
}

#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "hashentry.h"

#define BucketsSize 32
#define CONVERT(m) (void*)&m
#define BucketsSize 32

typedef struct stu
{
    int no;
    char name[32];
    int age;
}stu_t;
void keyForeachFunc(void* item)
{
    printf("key is %d\n", *((int*)item));
}

void valueForeachFunc(void* item)
{
    stu_t* stu = (stu_t*)item;

    printf("no is %d\n", stu->no);
    printf("name is %s\n", stu->name);
    printf("age is %d\n", stu->age);
    printf("\n");
}

int hash_str(int buckets, void* key)
{
    char* num = (char*)key;
    int index = 0;

    while(num)
    {
        index = *num + 4*index;
        num++;
    }

    return index % buckets;
}

int hash_int(int buckets, void* key)
{
    int* n = (int*)key;
    return (*n) % buckets;
}

int main(void)
{
    stu_t stu_arr[] =
    {
        {1234, "AAA", 19},
        {2345, "BBB", 20},
        {3456, "CCC", 21},
        {4567, "DDD", 22}
    };

    hash_t* hash = Hash_alloc(BucketsSize, hash_int);

    int i = 0;
    for(i = 0; i < 4; ++i)
    {
        Hash_add_entry(hash, &(stu_arr[i].no), sizeof(stu_arr[i].no),
                       &(stu_arr[i]), sizeof(stu_arr[i]));
    }

    printf("foreach\n");
    Hash_foreach(hash, keyForeachFunc, valueForeachFunc);

    printf("find\n");
    stu_t* stu = (stu_t*)Hash_find_entry(hash, &(stu_arr[3].no), sizeof(stu_arr[3].no));
    assert(stu);
    valueForeachFunc(stu);

    Hash_free_entry(hash, &(stu_arr[1].no), sizeof(stu_arr[1].no));
    printf("after free foreach\n");
    Hash_foreach(hash, keyForeachFunc, valueForeachFunc);

    Hash_destroy(hash);

    return 0;
}
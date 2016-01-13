//skiplist.h

#include <stdlib.h>

#define SKIPLIST_MAXLEVEL 32
#define SKIPLIST_P 0.25

typedef struct slobj {
    char *ptr;
    size_t length;
} slobj;

typedef struct skiplistNode {
    slobj* obj;
    double score;
    struct skiplistNode *backward;
    struct skiplistLevel {
        struct skiplistNode *forward;
        unsigned int span;
    }level[];
} skiplistNode;

typedef struct skiplist {
    struct skiplistNode *header, *tail;
    unsigned long length;
    int level;
} skiplist;

typedef void (*slDeleteCb) (void *ud, slobj *obj);
slobj* slCreateObj(const char* ptr, size_t length);
void slFreeObj(slobj *obj);

skiplist *slCreate(void);
void slFree(skiplist *sl);
void slDump(skiplist *sl);

void slInsert(skiplist *sl, double score, slobj *obj);
int slDelete(skiplist *sl, double score, slobj *obj);
unsigned long slDeleteByRank(skiplist *sl, unsigned int start, unsigned int end, slDeleteCb cb, void* ud);

unsigned long slGetRank(skiplist *sl, double score, slobj *o);
skiplistNode* slGetNodeByRank(skiplist *sl, unsigned long rank);

skiplistNode *slFirstInRange(skiplist *sl, double min, double max);
skiplistNode *slLastInRange(skiplist *sl, double min, double max);

// skiplist.c
/*
 *  author: xjdrew
 *  date: 2014-06-03 20:38
 */

// skiplist similar with the version in redis
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "skiplist.h"


skiplistNode *slCreateNode(int level, double score, slobj *obj) {
    skiplistNode *n = malloc(sizeof(*n) + level * sizeof(struct skiplistLevel));
    n->score = score;
    n->obj   = obj;
    return n;
}

skiplist *slCreate(void) {
    int j;
    skiplist *sl;

    sl = malloc(sizeof(*sl));
    sl->level = 1;
    sl->length = 0;
    sl->header = slCreateNode(SKIPLIST_MAXLEVEL, 0, NULL);
    for (j=0; j < SKIPLIST_MAXLEVEL; j++) {
        sl->header->level[j].forward = NULL;
        sl->header->level[j].span = 0;
    }
    sl->header->backward = NULL;
    sl->tail = NULL;
    return sl;
}

slobj* slCreateObj(const char* ptr, size_t length) {
    slobj *obj = malloc(sizeof(*obj));
    obj->ptr    = malloc(length + 1);

    if(ptr) {
        memcpy(obj->ptr, ptr, length);
    }
    obj->ptr[length] = '\0';

    obj->length = length;
    return obj;
}

void slFreeObj(slobj *obj) {
    free(obj->ptr);
    free(obj);
}

void slFreeNode(skiplistNode *node) {
    slFreeObj(node->obj);
    free(node);
}

void slFree(skiplist *sl) {
    skiplistNode *node = sl->header->level[0].forward, *next;

    free(sl->header);
    while(node) {
        next = node->level[0].forward;
        slFreeNode(node);
        node = next;
    }
    free(sl);
}

int slRandomLevel(void) {
    int level = 1;
    while((random() & 0xffff) < (SKIPLIST_P * 0xffff))
        level += 1;
    return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

int compareslObj(slobj *a, slobj *b) {
    int cmp = memcmp(a->ptr, b->ptr, a->length <= b->length ? a->length : b->length);
    if(cmp == 0) return a->length - b->length;
    return cmp;
}

int equalslObj(slobj *a, slobj *b) {
    return compareslObj(a, b) == 0;
}

void slInsert(skiplist *sl, double score, slobj *obj) {
    skiplistNode *update[SKIPLIST_MAXLEVEL], *x;
    unsigned int rank[SKIPLIST_MAXLEVEL];
    int i, level;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        /* store rank that is crossed to reach the insert position */
        rank[i] = i == (sl->level-1) ? 0 : rank[i+1];
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                compareslObj(x->level[i].forward->obj,obj) < 0))) {
            rank[i] += x->level[i].span;
            x = x->level[i].forward;
        }
        update[i] = x;
    }
    /* we assume the key is not already inside, since we allow duplicated
     * scores, and the re-insertion of score and redis object should never
     * happen since the caller of slInsert() should test in the hash table
     * if the element is already inside or not. */
    level = slRandomLevel();
    if (level > sl->level) {
        for (i = sl->level; i < level; i++) {
            rank[i] = 0;
            update[i] = sl->header;
            update[i]->level[i].span = sl->length;
        }
        sl->level = level;
    }
    x = slCreateNode(level,score,obj);
    for (i = 0; i < level; i++) {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;

        /* update span covered by update[i] as x is inserted here */
        x->level[i].span = update[i]->level[i].span - (rank[0] - rank[i]);
        update[i]->level[i].span = (rank[0] - rank[i]) + 1;
    }

    /* increment span for untouched levels */
    for (i = level; i < sl->level; i++) {
        update[i]->level[i].span++;
    }

    x->backward = (update[0] == sl->header) ? NULL : update[0];
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        sl->tail = x;
    sl->length++;
}

/* Internal function used by slDelete, slDeleteByScore */
void slDeleteNode(skiplist *sl, skiplistNode *x, skiplistNode **update) {
    int i;
    for (i = 0; i < sl->level; i++) {
        if (update[i]->level[i].forward == x) {
            update[i]->level[i].span += x->level[i].span - 1;
            update[i]->level[i].forward = x->level[i].forward;
        } else {
            update[i]->level[i].span -= 1;
        }
    }
    if (x->level[0].forward) {
        x->level[0].forward->backward = x->backward;
    } else {
        sl->tail = x->backward;
    }
    while(sl->level > 1 && sl->header->level[sl->level-1].forward == NULL)
        sl->level--;
    sl->length--;
}

/* Delete an element with matching score/object from the skiplist. */
int slDelete(skiplist *sl, double score, slobj *obj) {
    skiplistNode *update[SKIPLIST_MAXLEVEL], *x;
    int i;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                compareslObj(x->level[i].forward->obj,obj) < 0)))
            x = x->level[i].forward;
        update[i] = x;
    }
    /* We may have multiple elements with the same score, what we need
     * is to find the element with both the right score and object. */
    x = x->level[0].forward;
    if (x && score == x->score && equalslObj(x->obj,obj)) {
        slDeleteNode(sl, x, update);
        slFreeNode(x);
        return 1;
    } else {
        return 0; /* not found */
    }
    return 0; /* not found */
}

/* Delete all the elements with rank between start and end from the skiplist.
 * Start and end are inclusive. Note that start and end need to be 1-based */
unsigned long slDeleteByRank(skiplist *sl, unsigned int start, unsigned int end, slDeleteCb cb, void* ud) {
    skiplistNode *update[SKIPLIST_MAXLEVEL], *x;
    unsigned long traversed = 0, removed = 0;
    int i;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        while (x->level[i].forward && (traversed + x->level[i].span) < start) {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        update[i] = x;
    }

    traversed++;
    x = x->level[0].forward;
    while (x && traversed <= end) {
        skiplistNode *next = x->level[0].forward;
        slDeleteNode(sl,x,update);
        cb(ud, x->obj);
        slFreeNode(x);
        removed++;
        traversed++;
        x = next;
    }
    return removed;
}

/* Find the rank for an element by both score and key.
 * Returns 0 when the element cannot be found, rank otherwise.
 * Note that the rank is 1-based due to the span of sl->header to the
 * first element. */
unsigned long slGetRank(skiplist *sl, double score, slobj *o) {
    skiplistNode *x;
    unsigned long rank = 0;
    int i;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        while (x->level[i].forward &&
            (x->level[i].forward->score < score ||
                (x->level[i].forward->score == score &&
                compareslObj(x->level[i].forward->obj,o) <= 0))) {
            rank += x->level[i].span;
            x = x->level[i].forward;
        }

        /* x might be equal to sl->header, so test if obj is non-NULL */
        if (x->obj && equalslObj(x->obj, o)) {
            return rank;
        }
    }
    return 0;
}

/* Finds an element by its rank. The rank argument needs to be 1-based. */
skiplistNode* slGetNodeByRank(skiplist *sl, unsigned long rank) {
    if(rank == 0 || rank > sl->length) {
        return NULL;
    }

    skiplistNode *x;
    unsigned long traversed = 0;
    int i;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        while (x->level[i].forward && (traversed + x->level[i].span) <= rank)
        {
            traversed += x->level[i].span;
            x = x->level[i].forward;
        }
        if (traversed == rank) {
            return x;
        }
    }

    return NULL;
}

/* range [min, max], left & right both include */
/* Returns if there is a part of the zset is in range. */
int slIsInRange(skiplist *sl, double min, double max) {
    skiplistNode *x;

    /* Test for ranges that will always be empty. */
    if(min > max) {
        return 0;
    }
    x = sl->tail;
    if (x == NULL || x->score < min)
        return 0;

    x = sl->header->level[0].forward;
    if (x == NULL || x->score > max)
        return 0;
    return 1;
}

/* Find the first node that is contained in the specified range.
 * Returns NULL when no element is contained in the range. */
skiplistNode *slFirstInRange(skiplist *sl, double min, double max) {
    skiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!slIsInRange(sl,min, max)) return NULL;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        /* Go forward while *OUT* of range. */
        while (x->level[i].forward && x->level[i].forward->score < min)
                x = x->level[i].forward;
    }

    /* This is an inner range, so the next node cannot be NULL. */
    x = x->level[0].forward;
    return x;
}

/* Find the last node that is contained in the specified range.
 * Returns NULL when no element is contained in the range. */
skiplistNode *slLastInRange(skiplist *sl, double min, double max) {
    skiplistNode *x;
    int i;

    /* If everything is out of range, return early. */
    if (!slIsInRange(sl, min, max)) return NULL;

    x = sl->header;
    for (i = sl->level-1; i >= 0; i--) {
        /* Go forward while *IN* range. */
        while (x->level[i].forward &&
            x->level[i].forward->score <= max)
                x = x->level[i].forward;
    }

    /* This is an inner range, so this node cannot be NULL. */
    return x;
}

void slDump(skiplist *sl) {
    skiplistNode *x;
    int i;

    x = sl->header;
    i = 0;
    while(x->level[0].forward) {
        x = x->level[0].forward;
        i++;
        printf("node %d: score:%f, member:%s\n", i, x->score, x->obj->ptr);
    }
}


// lua-skiplist.c
/*
 *  author: xjdrew
 *  date: 2014-06-03 20:38
 */

#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "skiplist.h"

static inline skiplist*
_to_skiplist(lua_State *L) {
    skiplist **sl = lua_touserdata(L, 1);
    if(sl==NULL) {
        luaL_error(L, "must be skiplist object");
    }
    return *sl;
}

static int
_insert(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    double score = luaL_checknumber(L, 2);
    luaL_checktype(L, 3, LUA_TSTRING);
    size_t len;
    const char* ptr = lua_tolstring(L, 3, &len);
    slobj *obj = slCreateObj(ptr, len);
    slInsert(sl, score, obj);
    return 0;
}

static int
_delete(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    double score = luaL_checknumber(L, 2);
    luaL_checktype(L, 3, LUA_TSTRING);
    slobj obj;
    obj.ptr = lua_tolstring(L, 3, &obj.length);
    lua_pushboolean(L, slDelete(sl, score, &obj));
    return 1;
}

static void
_delete_rank_cb(void* ud, slobj *obj) {
    lua_State *L = (lua_State*)ud;
    lua_pushvalue(L, 4);
    lua_pushlstring(L, obj->ptr, obj->length);
    lua_call(L, 1, 0);
}

static int
_delete_by_rank(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    unsigned int start = luaL_checkunsigned(L, 2);
    unsigned int end = luaL_checkunsigned(L, 3);
    luaL_checktype(L, 4, LUA_TFUNCTION);
    if (start > end) {
        unsigned int tmp = start;
        start = end;
        end = tmp;
    }

    lua_pushunsigned(L, slDeleteByRank(sl, start, end, _delete_rank_cb, L));
    return 1;
}

static int
_get_count(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    lua_pushunsigned(L, sl->length);
    return 1;
}

static int
_get_rank(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    double score = luaL_checknumber(L, 2);
    luaL_checktype(L, 3, LUA_TSTRING);
    slobj obj;
    obj.ptr = lua_tolstring(L, 3, &obj.length);

    unsigned long rank = slGetRank(sl, score, &obj);
    if(rank == 0) {
        return 0;
    }

    lua_pushunsigned(L, rank);

    return 1;
}

static int
_get_rank_range(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    unsigned long r1 = luaL_checkunsigned(L, 2);
    unsigned long r2 = luaL_checkunsigned(L, 3);
    int reverse, rangelen;
    if(r1 <= r2) {
        reverse = 0;
        rangelen = r2 - r1 + 1;
    } else {
        reverse = 1;
        rangelen = r1 - r2 + 1;
    }

    skiplistNode* node = slGetNodeByRank(sl, r1);
    lua_createtable(L, rangelen, 0);
    int n = 0;
    while(node && n < rangelen) {
        n++;

        lua_pushlstring(L, node->obj->ptr, node->obj->length);
        lua_rawseti(L, -2, n);
        node = reverse? node->backward : node->level[0].forward;
    } 
    return 1;
}

static int
_get_score_range(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    double s1 = luaL_checknumber(L, 2);
    double s2 = luaL_checknumber(L, 3);
    int reverse; 
    skiplistNode *node;

    if(s1 <= s2) {
        reverse = 0;
        node = slFirstInRange(sl, s1, s2);
    } else {
        reverse = 1;
        node = slLastInRange(sl, s2, s1);
    }

    lua_newtable(L);
    int n = 0;
    while(node) {
        if(reverse) {
            if(node->score < s2) break;
        } else {
            if(node->score > s2) break;
        }
        n++;

        lua_pushlstring(L, node->obj->ptr, node->obj->length);
        lua_rawseti(L, -2, n);

        node = reverse? node->backward:node->level[0].forward;
    }
    return 1;
}

static int
_dump(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    slDump(sl);
    return 0;
}

static int
_new(lua_State *L) {
    skiplist *psl = slCreate();

    skiplist **sl = (skiplist**) lua_newuserdata(L, sizeof(skiplist*));
    *sl = psl;
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_setmetatable(L, -2);
    return 1;
}

static int
_release(lua_State *L) {
    skiplist *sl = _to_skiplist(L);
    printf("collect sl:%p\n", sl);
    slFree(sl);
    return 0;
}

int luaopen_skiplist_c(lua_State *L) {
    luaL_checkversion(L);

    luaL_Reg l[] = {
        {"insert", _insert},
        {"delete", _delete},
        {"delete_by_rank", _delete_by_rank},

        {"get_count", _get_count},
        {"get_rank", _get_rank},
        {"get_rank_range", _get_rank_range},
        {"get_score_range", _get_score_range},

        {"dump", _dump},
        {NULL, NULL}
    };

    lua_createtable(L, 0, 2);

    luaL_newlib(L, l);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, _release);
    lua_setfield(L, -2, "__gc");

    lua_pushcclosure(L, _new, 1);
    return 1;
}

// Makefile

all: skiplist.so

skiplist.so: skiplist.h skiplist.c lua-skiplist.c
    gcc -g3 -O0 -Wall -fPIC --shared $^ -o $@

test:
    lua test_sl.lua

clean:
    -rm skiplist.so

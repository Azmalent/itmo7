#ifndef TEST_ROUGH_LIST
#define TEST_ROUGH_LIST

#include <pthread.h>

#include "list.hpp"

typedef struct RoughNode {
    int key;
    int value;
    struct RoughNode* next;
} RoughNode;

typedef struct {
    RoughNode* root;
    pthread_mutex_t mutex;
} RoughList;

namespace Rough
{
    char insert(RoughList* list, int key, int value);
    char remove(RoughList* list, int key);
    FindResult find(RoughList* list, int key);

    void test();
}

#endif
#ifndef TEST_THIN_LIST
#define TEST_THIN_LIST

#include <pthread.h>

#include "list.hpp"

typedef struct ThinNode {
    int key;
    int value;
    pthread_mutex_t mutex;
    struct ThinNode* next;
} ThinNode;

typedef struct {
    ThinNode* root;
} ThinList;

namespace Thin
{
    char insert(ThinList* list, int key, int value);
    char remove(ThinList* list, int key);
    FindResult find(ThinList* list, int key);

    void test();
}

#endif
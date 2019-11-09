#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

#include "rough.hpp"

char Rough::insert(RoughList* list, int key, int value)
{
    char success = 0;
    char found = 0;

    pthread_mutex_lock( &(list->mutex) );

    RoughNode* node = list->root;
    RoughNode* last = NULL;
    while (node != NULL)
    {
        if (node->key == key)
        { 
            found = 1;
            break;
        }
        last = node;
        node = node->next;
    }

    if (!found)
    {
        RoughNode* newNode = (RoughNode*) malloc( sizeof(RoughNode) );
        *newNode = { key, value, NULL };

        if (last == NULL) list->root = newNode;
        else last->next = newNode;
        success = 1;
    }

    pthread_mutex_unlock( &(list->mutex) );
    return success;
}

char Rough::remove(RoughList* list, int key)
{
    char success = 0;

    pthread_mutex_lock( &(list->mutex) );

    RoughNode* node = list->root;
    RoughNode* last = NULL;
    while (node != NULL)
    {
        if (node->key == key) 
        {
            if (last == NULL) list->root = node->next;
            else last->next = node->next;

            free(node);
            success = 1;
            break;
        }

        last = node;
        node = node->next;
    }

    pthread_mutex_unlock( &(list->mutex) );
    return success;
}

FindResult Rough::find(RoughList* list, int key)
{
    FindResult result = { 0, 0 };

    pthread_mutex_lock( &(list->mutex) );

    RoughNode* node = list->root;
    while (node != NULL)
    {
        if (node->key == key) 
        {
            result = { 1, node->value };
            break;
        }

        node = node->next;
    }

    pthread_mutex_unlock( &(list->mutex) );
    return result;
}

void Rough::test()
{
    RoughList list = { NULL, PTHREAD_MUTEX_INITIALIZER };
    insert(&list, 1, 2);

    {
        FindResult result = find(&list, 1);
        assert(result.exists);
        assert(result.value == 2);
    }

    {
        FindResult result = find(&list, 0);
        assert(!result.exists);
    }

    insert(&list, 0, 1);

    {
        FindResult result = find(&list, 0);
        assert(result.exists);
        assert(result.value == 1);
    }

    insert(&list, 2, 3);

    {
        FindResult result = find(&list, 2);
        assert(result.exists);
        assert(result.value == 3);
    }

    assert(remove(&list, 1));

    {
        FindResult result = find(&list, 1);
        assert(!result.exists);
    }

    assert(remove(&list, 0));
    assert(remove(&list, 2));

    {
        FindResult result = find(&list, 0);
        assert(!result.exists);
    }

    {
        FindResult result = find(&list, 1);
        assert(!result.exists);
    }

    {
        FindResult result = find(&list, 2);
        assert(!result.exists);
    }

    puts("Rough tests succeeded!");
}


#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

#include "thin.hpp"

char Thin::insert(ThinList* list, int key, int value)
{
    char success = 0;
    char found = 0;

    ThinNode* node = list->root;
    ThinNode* last = NULL;
    pthread_mutex_t* mutex;
    while (node != NULL)
    {
        mutex = &(node->mutex);
        pthread_mutex_lock(mutex);

        if (node->key == key)
        { 
            found = 1;
            pthread_mutex_unlock(mutex);
            break;
        }
        last = node;
        node = node->next;
    }

    if (!found)
    {        
        ThinNode* newNode = (ThinNode*) malloc( sizeof(ThinNode) );
        *newNode = { key, value, PTHREAD_MUTEX_INITIALIZER, NULL };

        if (last == NULL) list->root = newNode;
        else last->next = newNode;
        success = 1;

        pthread_mutex_unlock(mutex);
    }

    return success;
}

char Thin::remove(ThinList* list, int key)
{
    char success = 0;

    ThinNode* node = list->root;
    ThinNode* last = NULL;
    pthread_mutex_t* mutex;
    while (node != NULL)
    {        
        mutex = &(node->mutex);
        pthread_mutex_lock(mutex);

        if (node->key == key) 
        {
            if (last == NULL) list->root = node->next;
            else last->next = node->next;

            free(node);
            success = 1;

            pthread_mutex_unlock(mutex);
            break;
        }

        last = node;
        node = node->next;

        pthread_mutex_unlock(mutex);
    }

    return success;
}

FindResult Thin::find(ThinList* list, int key)
{
    FindResult result = { 0, 0 };

    ThinNode* node = list->root;
    pthread_mutex_t* mutex;
    while (node != NULL)
    {
        mutex = &(node->mutex);
        pthread_mutex_lock(mutex);

        if (node->key == key) 
        {
            result = { 1, node->value };
            pthread_mutex_unlock(mutex);
            break;
        }

        node = node->next;
        pthread_mutex_unlock(mutex);
    }

    return result;
}

void Thin::test()
{
    ThinList list = { NULL };
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

    puts("Thin tests succeeded!");
}
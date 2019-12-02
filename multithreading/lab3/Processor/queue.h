#ifndef LAB3_QUEUE
#define LAB3_QUEUE

#include <pthread.h>

#define NEW_QUEUE { NULL, PTHREAD_MUTEX_INITIALIZER }

typedef struct node_t {
    void* value;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* first;
    pthread_mutex_t mutex;
} queue_t;

void enqueue(queue_t* queue, void* data)
{
    node_t* new_node = malloc( sizeof(node_t) );
    new_node->value = data;
    new_node->next = NULL;

    pthread_mutex_lock( &(queue->mutex) );
    {
        node_t* node = queue->first;
        if (node != NULL)
        {
            while (node->next != NULL) node = node->next;
            node->next = new_node;
        }
        else queue->first = new_node;
    }
    pthread_mutex_unlock( &(queue->mutex) );
}

void* dequeue(queue_t* queue)
{
    void* value = NULL;

    pthread_mutex_lock( &(queue->mutex) );
    {
        node_t* first = queue->first;
        if (first != NULL) 
        {
            value = first->value;
            queue->first = first->next;
            free(first);
        }
    }
    pthread_mutex_unlock( &(queue->mutex) );

    return value;
}

#endif
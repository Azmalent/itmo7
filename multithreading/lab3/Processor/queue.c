#include <pthread.h>
#include <stdlib.h>

#include "include/queue.h"

void enqueue(queue_t* queue, void* data)
{
    node_t* new_node = malloc( sizeof(node_t) );
    *new_node = (node_t) { data, NULL };

    pthread_mutex_lock( &(queue->mutex) );
    
    node_t* node = queue->first;
    if (node != NULL)
    {
        while (node->next != NULL) node = node->next;
        node->next = new_node;
    }
    else queue->first = new_node;

    pthread_mutex_unlock( &(queue->mutex) );
}

void* dequeue(queue_t* queue)
{
    void* value = NULL;
    pthread_mutex_lock( &(queue->mutex) );
    
    node_t* first = queue->first;
    if (first != NULL) 
    {
        value = first->value;
        queue->first = first->next;
        free(first);
    }
    
    pthread_mutex_unlock( &(queue->mutex) );
    return value;
}
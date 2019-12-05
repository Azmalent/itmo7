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

void enqueue(queue_t* queue, void* data);

void* dequeue(queue_t* queue);

#endif
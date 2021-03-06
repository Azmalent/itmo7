#ifndef TEST_QUEUE
#define TEST_QUEUE

#include <pthread.h>

typedef struct QueueNode {
    void* value;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* first;
    pthread_mutex_t mutex;
} ConcurrentQueue;

namespace Queue
{
    void enqueue(ConcurrentQueue* queue, void* value);
    void* dequeue(ConcurrentQueue* queue);
}

#endif
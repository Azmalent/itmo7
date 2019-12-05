#include <pthread.h>
#include <stdlib.h>

#include "include/per_type.h"
#include "include/message.h"
#include "include/queue.h"
#include "include/tasks.h"

static pthread_t fib_thread;
static queue_t fib_queue = NEW_QUEUE;
void* fib_thread_task(void* arg)
{
    while(true)
    {
        message_t* msg = dequeue(&fib_queue);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        
        run_fib_task(msg); 
    }

    return NULL;
}

static pthread_t pow_thread;
static queue_t pow_queue = NEW_QUEUE;
void* pow_thread_task(void* arg)
{
    while(true)
    {
        message_t* msg = dequeue(&pow_queue);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        
        run_pow_task(msg); 
    }

    return NULL;
}

static pthread_t sort_thread;
static queue_t sort_queue = NEW_QUEUE;
void* sort_thread_task(void* arg)
{
    while(true)
    {
        message_t* msg = dequeue(&sort_queue);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        
        run_sort_task(msg); 
    }

    return NULL;

    return NULL;
}


void* reader_per_type(void* arg)
{
    pthread_create(&fib_thread, NULL, fib_thread_task, NULL);
    pthread_create(&pow_thread, NULL, pow_thread_task, NULL);
    pthread_create(&sort_thread, NULL, sort_thread_task, NULL);

    do
    {
        message_t* msg = malloc(sizeof(message_t));
        *msg = read_message();

        switch(msg->type)
        {
            case FIBONACCI: enqueue(&fib_queue, msg);   break;
            case POWER:     enqueue(&pow_queue, msg);   break;
            case SORT:      enqueue(&sort_queue, msg);  break;
            case STOP:      stop_recieved = true;       break;                
        }
    } while (!stop_recieved);

    pthread_join(fib_thread, NULL);
    pthread_join(pow_thread, NULL);
    pthread_join(sort_thread, NULL);

    return NULL;
}
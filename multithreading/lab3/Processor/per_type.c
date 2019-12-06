#include <pthread.h>
#include <stdlib.h>

#include "include/per_type.h"
#include "include/metrics.h"
#include "include/message.h"
#include "include/queue.h"
#include "include/tasks.h"

static vector_t reader_metric = NEW_VECTOR;
static vector_t wait_metric = NEW_VECTOR;
static vector_t process_metric = NEW_VECTOR;

static pthread_t fib_thread;
static queue_t fib_queue = NEW_QUEUE;
void* fib_thread_task(void* arg)
{
    while(true)
    {
        int wait_time;
        message_t* msg = dequeue(&fib_queue, &wait_time);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        vec_append(&wait_metric, wait_time); 
        
        int process_time = run_fib_task(msg);
        vec_append(&process_metric, process_time); 
    }

    return NULL;
}

static pthread_t pow_thread;
static queue_t pow_queue = NEW_QUEUE;
void* pow_thread_task(void* arg)
{
    while(true)
    {
        int wait_time;
        message_t* msg = dequeue(&pow_queue, &wait_time);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        vec_append(&wait_metric, wait_time); 
        
        int process_time = run_pow_task(msg); 
        vec_append(&process_metric, process_time);
    }

    return NULL;
}

static pthread_t sort_thread;
static queue_t sort_queue = NEW_QUEUE;
void* sort_thread_task(void* arg)
{
    while(true)
    {
        int wait_time;
        message_t* msg = dequeue(&sort_queue, &wait_time);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        vec_append(&wait_metric, wait_time); 
        
        int process_time = run_sort_task(msg); 
        vec_append(&process_metric, process_time);
    }

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

        int read_time;
        *msg = read_message(&read_time);
        vec_append(&reader_metric, read_time);

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

void metrics_per_type(int* read, int* wait, int* work)
{
    *read = percentile(reader_metric, PERCENTILE);
    vec_clear(&reader_metric);

    *wait = percentile(wait_metric, PERCENTILE);
    vec_clear(&wait_metric);

    *work = percentile(process_metric, PERCENTILE);
    vec_clear(&process_metric);
}

void finalize_per_type()
{
    free(reader_metric.data);
    free(wait_metric.data);
    free(process_metric.data);
}
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/message.h"
#include "include/metrics.h"
#include "include/queue.h"
#include "include/tasks.h"
#include "include/thread_pool.h"

static vector_t reader_metric = NEW_VECTOR;
static vector_t wait_metric = NEW_VECTOR;
static vector_t process_metric = NEW_VECTOR;

static int thread_count;
static pthread_t* thread_pool;
static queue_t thread_pool_queue;

void* thread_pool_worker(void* arg)
{
    while(true)
    {
        int wait_time;
        message_t* msg = dequeue(&thread_pool_queue, &wait_time);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        vec_append(&wait_metric, wait_time); 
        
        int process_time = run_task(msg);
        vec_append(&process_metric, process_time);
    }

    return NULL;
}

void init_thread_pool(int n)
{
    if (n < 1 || n > MAX_THREAD_POOL_SIZE)
    {
        printf("Ошибка: некорректное количество потоков. Требуется ввести положительное число не более %i.",
            MAX_THREAD_POOL_SIZE);
        exit(EXIT_FAILURE);
    }
    
    thread_count = n;
    thread_pool = malloc(n * sizeof(pthread_t)); 
}

void* reader_thread_pool(void* arg)
{
    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_pool_worker, NULL);
    }

    do
    {
        message_t* msg = malloc( sizeof(message_t) );
        
        int read_time;
        *msg = read_message(&read_time);
        vec_append(&reader_metric, read_time);

        if (msg->type == STOP) stop_recieved = true;
        else enqueue(&thread_pool_queue, msg);
    } while (!stop_recieved);

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }

    return NULL;
}

void metrics_thread_pool(int* read, int* wait, int* work)
{
    *read = percentile(reader_metric, PERCENTILE);
    vec_clear(&reader_metric);

    *wait = percentile(wait_metric, PERCENTILE);
    vec_clear(&wait_metric);

    *work = percentile(process_metric, PERCENTILE);
    vec_clear(&process_metric);
}

void finalize_thread_pool()
{
    free(thread_pool);
    
    free(reader_metric.data);
    free(wait_metric.data);
    free(process_metric.data);
}
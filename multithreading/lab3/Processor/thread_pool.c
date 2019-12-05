#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/message.h"
#include "include/queue.h"
#include "include/tasks.h"
#include "include/thread_pool.h"

static int thread_count;
static pthread_t* thread_pool;
static queue_t thread_pool_queue;

void* thread_pool_worker(void* arg)
{
    while(true)
    {
        message_t* msg = dequeue(&thread_pool_queue);
        if (msg == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        
        run_task(msg); 
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
        *msg = read_message();

        if (msg->type == STOP) stop_recieved = true;
        else enqueue(&thread_pool_queue, msg);
    } while (!stop_recieved);

    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(thread_pool[i], NULL);
    }

    free(thread_pool);
    return NULL;
}
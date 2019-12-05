#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/per_task.h"
#include "include/queue.h"
#include "include/tasks.h"
#include "include/types.h"

static volatile int thread_count = 0;

void* runner_per_task(void* arg)
{
    message_t* msg = (message_t*) arg;
    run_task(msg);
    thread_count--;

    return NULL;
}

void* reader_per_task(void* arg)
{
    do
    {
        message_t* msg = malloc( sizeof(message_t) );
        *msg = read_message();

        if (msg->type == STOP) stop_recieved = true;
        else
        {
            thread_count++;
            pthread_t thread;
            pthread_create(&thread, NULL, runner_per_task, msg);
        }
    } while (!stop_recieved);
    
    while (thread_count > 0);
    return NULL;
}
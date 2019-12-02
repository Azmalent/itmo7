#ifndef LAB3_PER_TASK
#define LAB3_PER_TASK

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#include "messages.h"
#include "queue.h"
#include "writer.h"

static int thread_count;
static bool stopped;

void* runner_per_task(void* arg)
{
    message_t* msg = (message_t*) arg;

    writer_arg_t* writer_args = get_task_stats(msg);
    enqueue(&writer_queue, writer_args);

    thread_count--;
}

void* reader_per_task(void* arg)
{
    thread_count = 0;

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
}

#endif
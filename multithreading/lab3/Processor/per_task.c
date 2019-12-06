#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/per_task.h"
#include "include/queue.h"
#include "include/metrics.h"
#include "include/tasks.h"
#include "include/types.h"

static vector_t reader_metric = NEW_VECTOR;
static vector_t process_metric = NEW_VECTOR;

static volatile int thread_count = 0;

void* runner_per_task(void* arg)
{
    message_t* msg = (message_t*) arg;
    
    int process_time = run_task(msg);
    vec_append(&process_metric, process_time);

    thread_count--;

    return NULL;
}

void* reader_per_task(void* arg)
{
    do
    {
        message_t* msg = malloc( sizeof(message_t) );

        int read_time;
        *msg = read_message(&read_time);
        vec_append(&reader_metric, read_time);

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

void metrics_per_task(int* read, int* work)
{
    *read = percentile(reader_metric, PERCENTILE);
    vec_clear(&reader_metric);

    *work = percentile(process_metric, PERCENTILE);
    vec_clear(&process_metric);
}

void finalize_per_task()
{
    free(reader_metric.data);
    free(process_metric.data);
}
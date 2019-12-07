#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "include/message.h"
#include "include/queue.h"
#include "include/metrics.h"
#include "include/writer.h"

static vector_t wait_metric = NEW_VECTOR;
static vector_t write_metric = NEW_VECTOR;

queue_t writer_queue = NEW_QUEUE;

void write_line(FILE* file, pthread_t thread_id, message_t msg, void* result)
{
    clock_t start_time = clock();

    fprintf(file, "Thread %lu: ", thread_id);
    switch(msg.type)
    {
        case FIBONACCI: 
            fprintf(file, "fib %d -> %ld\n", *(msg.data), *((long*) result) ); 
            break;
        case POWER:     
            fprintf(file, "pow %d %d -> %e\n", msg.data[0], msg.data[1], *((double*) result) );  
            break;
        case SORT:      
            fprintf(file, "sort [");
            for (int i = 0; i < msg.size; i++) fprintf(file, " %i", msg.data[i]);
            fprintf(file, " ] -> [");
            for (int i = 0; i < msg.size; i++) fprintf(file, " %i", ((int*) result)[i] ); 
            fprintf(file, " ]\n");
            break;
        default:
            break;
    }  

    fflush(file);

    int write_time = (clock() - start_time) * 1000000 / CLOCKS_PER_SEC;
    vec_append(&write_metric, write_time);
}

void* writer_thread_func(void* arg)
{
    FILE* file = fopen(OUT_FILE, "w");

    while (true)
    {
        int wait_time;
        writer_arg_t* writer_args = dequeue(&writer_queue, &wait_time);
        if (writer_args == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        vec_append(&wait_metric, wait_time);
        
        pthread_t tid = writer_args->thread_id;
        message_t* msg = writer_args->msg;
        void* result = writer_args->result;

        write_line(file, tid, *msg, result);
    
        free(msg->data);
        free(msg);
        free(result);
        free(writer_args);
    }
    
    fclose(file);

    return NULL;
}

void writer_metrics(int* wait, int* write)
{
    *wait = percentile(wait_metric, PERCENTILE);
    *write = percentile(write_metric, PERCENTILE);
}
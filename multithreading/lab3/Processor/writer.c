#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "include/message.h"
#include "include/queue.h"
#include "include/types.h"
#include "include/writer.h"

queue_t writer_queue = NEW_QUEUE;

void write_line(FILE* file, pthread_t thread_id, message_t msg, void* result, long mcs_elapsed)
{
    fprintf(file, "Thread %lu: ", thread_id);
    switch(msg.type)
    {
        case FIBONACCI: 
            fprintf(file, "fib %d -> %ld", *(msg.data), *((long*) result) ); 
            break;
        case POWER:     
            fprintf(file, "pow %d %d -> %e", msg.data[0], msg.data[1], *((double*) result) );  
            break;
        case SORT:      
            fprintf(file, "sort [");
            for (int i = 0; i < msg.size; i++) fprintf(file, " %i", msg.data[i]);
            fprintf(file, " ] -> [");
            for (int i = 0; i < msg.size; i++) fprintf(file, " %i", ((int*) result)[i] ); 
            fprintf(file, " ]");
            break;
        default:
            break;
    }  

    fprintf(file, " (%ld microseconds)\n", mcs_elapsed); 
    fflush(file);
}

void* writer_thread_func(void* arg)
{
    FILE* file = fopen(OUT_FILE, "w");

    unsigned long total_mcs = 0;

    while (true)
    {
        writer_arg_t* writer_args = dequeue(&writer_queue);
        if (writer_args == NULL)
        {
            if (stop_recieved) break;
            else continue;
        }
        
        pthread_t tid = writer_args->thread_id;
        message_t* msg = writer_args->msg;
        void* result = writer_args->result;
        long mcs = writer_args->mcs_elapsed;

        total_mcs += mcs;
        write_line(file, tid, *msg, result, mcs);
    
        free(msg->data);
        free(msg);
        free(result);
        free(writer_args);
    }
    
    fprintf(file, "TOTAL TIME: %lu microseconds\n", total_mcs);
    fclose(file);

    return NULL;
}
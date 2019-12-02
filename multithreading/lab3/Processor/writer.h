#ifndef LAB3_WRITER
#define LAB3_WRITER

#include <string.h>
#include <sys/time.h>

#include "queue.h"

#define OUT_FILE "results.txt"

typedef struct {
    pthread_t thread_id;
    message_t* msg;
    void* result;
    unsigned long mcs_elapsed;
} writer_arg_t;

pthread_t writer;
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
    }  

    fprintf(file, " (%ld microseconds)\n", mcs_elapsed); 
    fflush(file);
}

void* writer_func(void* arg)
{
    FILE* file = fopen(OUT_FILE, "w");

    unsigned long total_mcs = 0;

    while(!stop_recieved)
    {
        writer_arg_t* wdata = dequeue(&writer_queue);
        if (wdata == NULL) continue;
        
        total_mcs += wdata->mcs_elapsed;
        write_line(file, wdata->thread_id, *(wdata->msg), wdata->result, wdata->mcs_elapsed);
    
        free(wdata->msg->data);
        free(wdata->msg);
        free(wdata->result);
        free(wdata);
    }
    
    fprintf(file, "TOTAL TIME: %lu microseconds\n", total_mcs);

    fclose(file);
}

#endif
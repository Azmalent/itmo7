#ifndef LAB3_WRITER
#define LAB3_WRITER

#include "queue.h"

#define OUT_FILE "results.txt"

typedef struct {
    pthread_t thread_id;
    message_t* msg;
    void* result;
} writer_arg_t;

extern queue_t writer_queue;

void* writer_thread_func(void* arg);
void writer_metrics(int* wait, int* write);

#endif
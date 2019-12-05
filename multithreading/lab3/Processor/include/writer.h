#ifndef LAB3_WRITER
#define LAB3_WRITER

#include "queue.h"

#define OUT_FILE "results.txt"

extern queue_t writer_queue;

void* writer_thread_func(void* arg);

#endif
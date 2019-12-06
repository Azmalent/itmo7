#ifndef LAB3_THREAD_POOL
#define LAB3_THREAD_POOL

#include "vector.h"

#define MAX_THREAD_POOL_SIZE 25

void init_thread_pool();
void* reader_thread_pool(void* arg);
void metrics_thread_pool(int* read, int* wait, int* work);
void finalize_thread_pool();

#endif
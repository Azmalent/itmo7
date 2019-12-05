#ifndef LAB3_THREAD_POOL
#define LAB3_THREAD_POOL

#define MAX_THREAD_POOL_SIZE 1000

void init_thread_pool();
void* reader_thread_pool(void* arg);

#endif
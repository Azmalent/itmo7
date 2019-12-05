#ifndef LAB3_TYPES
#define LAB3_TYPES

#include <pthread.h>
#include <stdbool.h>

typedef enum {
    PER_TASK,
    PER_TYPE,
    THREAD_POOL
} strategy_t;

typedef enum {
    FIBONACCI,
    POWER,
    SORT,
    STOP
} task_t;

typedef struct {
    task_t type;
    int size;
    int* data;
} message_t;

typedef struct {
    pthread_t thread_id;
    message_t* msg;
    void* result;
    unsigned long mcs_elapsed;
} writer_arg_t;

#endif
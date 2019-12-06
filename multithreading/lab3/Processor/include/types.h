#ifndef LAB3_TYPES
#define LAB3_TYPES

#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

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

#endif
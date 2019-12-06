#ifndef LAB3_PER_TASK
#define LAB3_PER_TASK

#include "vector.h"

void* reader_per_task(void* arg);
void metrics_per_task(int* read, int* work);
void finalize_per_task();

#endif
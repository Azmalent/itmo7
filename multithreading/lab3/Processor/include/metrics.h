#ifndef LAB3_METRICS
#define LAB3_METRICS

#include "types.h"
#include "vector.h"

#define METRICS_FILE "metrics.txt"
#define PERCENTILE 0.75

int percentile(vector_t vec, double percent);

void* monitor_thread_func(void* arg);
void init_monitor(int sleep_interval, strategy_t work_strategy);

#endif
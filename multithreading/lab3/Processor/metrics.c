#define _POSIX_C_SOURCE 199309L

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/message.h"
#include "include/metrics.h"
#include "include/per_task.h"
#include "include/per_type.h"
#include "include/thread_pool.h"
#include "include/writer.h"

static int sleep_interval;
static strategy_t strategy;

int partition(int* array, int l, int r)
{
    int v = array[ (l + r) / 2 ];
    
    while (l <= r)
    {
        while (array[l] < v) l++;
        while (array[r] > v) r--;
        if (l >= r) break;
        
        int temp = array[l];
        array[l] = array[r];
        array[r] = temp;
        
        l++;
        r--;
    }

    return r;
 }

int order_statistic(int* array, int len, int k)
{
    int left = 0;
    int right = len - 1;
    while (true) {
        int mid = partition(array, left, right);

        if (mid == k) return array[mid];
        else if (k < mid) right = mid;
        else  left = mid + 1;
    }
}

int percentile(vector_t vec, double percent)
{
    pthread_mutex_lock( &(vec.mutex) );

    int k = (int) (vec.length * percent);
    int result = k > 0 ? order_statistic(vec.data, vec.length, k - 1) : 0;

    pthread_mutex_unlock( &(vec.mutex) );

    return result;
}

void init_monitor(int ms, strategy_t str)
{
    sleep_interval = ms;
    strategy = str;
}

void* monitor_thread_func(void* arg)
{
    FILE* file = fopen(METRICS_FILE, "w");

    struct timespec ts;
    ts.tv_sec = sleep_interval / 1000L;
    ts.tv_nsec = (sleep_interval % 1000) * 1000000L;

    int total_mcs = 0;

    do
    {
        nanosleep(&ts, NULL);
        total_mcs += sleep_interval;

        int read, wait_proc, proc, wait_write, write;
        switch (strategy)
        {
            case PER_TASK:
                metrics_per_task(&read, &proc);
                wait_proc = 0;
                break;
            case PER_TYPE:
                metrics_per_type(&read, &wait_proc, &proc);
                break;
            case THREAD_POOL:
                metrics_thread_pool(&read, &wait_proc, &proc);
                break;
        }

        writer_metrics(&wait_write, &write);
        if (read || wait_proc || proc || wait_write || write)
        {
            fprintf(file, "%i %i %i %i %i %i\n", total_mcs, read, wait_proc, proc, wait_write, write);
        }
        
        fflush(file);
    } while (!stop_recieved);
    
    fclose(file);

    return NULL;
}
#ifndef LAB3_TASKS
#define LAB3_TASKS

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "messages.h"
#include "writer.h"

long fib(int n)
{
    if (n == 0) return 0;

    int sign = 1;
    if (n < 0)
    {
        n = -n;
        if ((n & 1) == 0) sign = -1;
    }

    long previous = 0;
    long current = 1;
    for (int i = 0; i < n-2; i++)
    {
        long next = previous + current;
        previous = current;
        current = next;
    }

    return sign * (previous + current);
}

double power(int base, int pow)
{
    if (pow == 0) return 1;
    if (base == 0) return 0;

    bool positive = pow > 0;
    if (!positive) pow = -pow;
    
    double result = 1;
    for (int i = 0; i < pow; i++) result *= base;
    
    return positive ? result : (1 / result);
}

void bubble_sort(int* values, int n)
{
    bool swapped;
    do
    {
        swapped = false;
        for (int i = 0; i < n-1; i++)
        {
            if (values[i] > values[i+1])
            {
                int temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;

                swapped = true;
            }
        }
    } while (swapped);
}


void* run_task(message_t* msg)
{
    size_t num_bytes = msg->size * sizeof(int);
    void* data = malloc(num_bytes);

    long* data_long = (long*) data;
    double* data_double = (double*) data;
    
    switch (msg->type)
    {
        case FIBONACCI:
            data = realloc(data, sizeof(unsigned long)); 
            *((long*) data) = fib(msg->data[0]);
            break;
        case SORT:
            memcpy(data, msg->data, num_bytes);
            bubble_sort(data, msg->size);
            break;
        case POWER:
            *((double*) data) = power(msg->data[0], msg->data[1]);
            break;
        default: 
            free(data);
            return NULL;
    }
    
    return data;
}

writer_arg_t* get_task_stats(message_t* msg)
{
    struct timeval start_time, end_time; 

    gettimeofday(&start_time, NULL);
    void* result = run_task(msg);
    gettimeofday(&end_time, NULL);

    unsigned long mcs_elapsed = end_time.tv_usec - start_time.tv_usec;

    if (result == NULL) 
    {
        puts("Ошибка при обработке сообщения: неизвестная задача");
        exit(EXIT_FAILURE);
    }

    writer_arg_t* writer_args = malloc(sizeof(writer_arg_t));
    writer_args->thread_id = pthread_self();
    writer_args->msg = msg;
    writer_args->result = result;
    writer_args->mcs_elapsed = mcs_elapsed;

    return writer_args;
}

#endif
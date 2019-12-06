#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "include/message.h"
#include "include/types.h"
#include "include/writer.h"

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


unsigned long run_fib_task(message_t* msg)
{
    clock_t start_time = clock();

    long* result = malloc(sizeof(long));
    *result = fib(msg->data[0]);
    
    clock_t end_time = clock();

    writer_arg_t* writer_args = malloc(sizeof(writer_arg_t));
    *writer_args = (writer_arg_t) { pthread_self(), msg, result };
    enqueue(&writer_queue, writer_args);

    return (end_time - start_time) * 1000000 / CLOCKS_PER_SEC;
}

unsigned long run_pow_task(message_t* msg)
{
    clock_t start_time = clock();

    double* result = malloc(sizeof(double));
    *result = power(msg->data[0], msg->data[1]);
    
    clock_t end_time = clock();

    writer_arg_t* writer_args = malloc(sizeof(writer_arg_t));
    *writer_args = (writer_arg_t) { pthread_self(), msg, result };
    enqueue(&writer_queue, writer_args);

    return (end_time - start_time) * 1000000 / CLOCKS_PER_SEC;
}

unsigned long run_sort_task(message_t* msg)
{
    clock_t start_time = clock();

    size_t num_bytes = msg->size * sizeof(int);
    int* result = malloc(num_bytes);
    memcpy(result, msg->data, num_bytes);
    bubble_sort(result, msg->size);
    
    clock_t end_time = clock();

    writer_arg_t* writer_args = malloc(sizeof(writer_arg_t));
    *writer_args = (writer_arg_t) { pthread_self(), msg, result };
    enqueue(&writer_queue, writer_args);

    return (end_time - start_time) * 1000000 / CLOCKS_PER_SEC;
}


unsigned long run_task(message_t* msg)
{
    switch (msg->type)
    {
        case FIBONACCI: return run_fib_task(msg);  break;
        case POWER:     return run_pow_task(msg);  break;
        case SORT:      return run_sort_task(msg); break;
        default:        return -1;
    }
}
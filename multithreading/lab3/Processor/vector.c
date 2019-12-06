#include <stdlib.h>

#include "include/vector.h"

void vec_append(vector_t* vec, int value)
{
    pthread_mutex_lock( &(vec->mutex) );

    if (vec->data == NULL) vec->data = malloc(vec->max_length * sizeof(int));

    if (vec->length >= vec->max_length)
    {
        vec->max_length = vec->max_length * 2;
        size_t new_size = vec->max_length * sizeof(int);

        vec->data = realloc(vec->data, new_size);
    }

    vec->data[vec->length] = value;
    vec->length++;

    pthread_mutex_unlock( &(vec->mutex) );
}

void vec_clear(vector_t* vec)
{
    pthread_mutex_lock( &(vec->mutex) );
    vec->length = 0;
    pthread_mutex_unlock( &(vec->mutex) );
}
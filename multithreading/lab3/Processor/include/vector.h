#ifndef LAB3_VECTOR
#define LAB3_VECTOR

#include <pthread.h>

#define INITIAL_VECTOR_SIZE 25
#define NEW_VECTOR { PTHREAD_MUTEX_INITIALIZER, NULL, INITIAL_VECTOR_SIZE, 0 }

typedef struct {
    pthread_mutex_t mutex;
    int* data;
    int max_length;
    int length;
} vector_t;

void vec_append(vector_t* vector, int value);

#endif
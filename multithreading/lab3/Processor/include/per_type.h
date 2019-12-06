#ifndef LAB3_PER_TYPE
#define LAB3_PER_TYPE

#include "vector.h"

void* reader_per_type(void* arg);
void metrics_per_type(int* read, int* wait, int* work);
void finalize_per_type();

#endif
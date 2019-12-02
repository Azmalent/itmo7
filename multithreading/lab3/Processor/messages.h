#ifndef LAB3_TYPES
#define LAB3_TYPES

#include <stdlib.h>

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

bool stop_recieved = false;


message_t read_message()
{
    message_t msg = { -1, 0, NULL };

    read(0, &(msg.type), sizeof(task_t) + sizeof(int));

    if (msg.size > 0)
    {
        size_t num_bytes = msg.size * sizeof(int);
        msg.data = malloc(num_bytes);
        read(0, msg.data, num_bytes);
    }

    return msg;
}

#endif
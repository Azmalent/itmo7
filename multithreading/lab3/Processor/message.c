#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/message.h"
#include "include/types.h"

bool stop_recieved = false;

message_t read_message(int* mcs)
{
    message_t msg = { -1, 0, NULL };

    clock_t start_time = clock();

    size_t bytes_read = read(0, &(msg.type), sizeof(task_t) + sizeof(int));
    if (bytes_read != sizeof(task_t) + sizeof(int))
    {
        puts("Ошибка при чтении сообщения");
        exit(EXIT_FAILURE);
    }

    if (msg.type < FIBONACCI || msg.type > STOP)
    {
        printf("Ошибка при чтении сообщения: неизвестная задача %i\n", msg.type);
        exit(EXIT_FAILURE);
    }

    if (msg.size > 0)
    {
        size_t num_bytes = msg.size * sizeof(int);
        msg.data = malloc(num_bytes);
        bytes_read = read(0, msg.data, num_bytes);
        if (num_bytes != bytes_read)
        {
            puts("Ошибка при чтении сообщения");
            exit(EXIT_FAILURE);
        }
    }

    *mcs = (clock() - start_time) * 1000000 / CLOCKS_PER_SEC;

    return msg;
}
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/message.h"
#include "include/types.h"

bool stop_recieved = false;

void strict_read(int fd, void* buf, size_t num_bytes)
{
    int total_bytes_read = 0;
    char* char_buf = (char*) buf;

    while (total_bytes_read < num_bytes)
    {
        int bytes_read = read(fd, char_buf + total_bytes_read, num_bytes - total_bytes_read);
        if (bytes_read <= 0)
        {
            puts("Ошибка при чтении сообщения");
            exit(EXIT_FAILURE);
        }

        total_bytes_read += bytes_read;
    }
}

message_t read_message(int* mcs)
{
    message_t msg = { -1, 0, NULL };

    int read_time = 0;

    clock_t start_time = clock();
    strict_read(0, &(msg.type), sizeof(task_t) + sizeof(int));
    read_time += (clock() - start_time) * 1000000 / CLOCKS_PER_SEC;


    if (msg.type < FIBONACCI || msg.type > STOP)
    {
        printf("Ошибка при чтении сообщения: неизвестная задача %i\n", msg.type);
        exit(EXIT_FAILURE);
    }

    if (msg.size > 0)
    {
        size_t num_bytes = msg.size * sizeof(int);

        msg.data = malloc(num_bytes);
        
        start_time = clock();
        strict_read(0, msg.data, num_bytes);
        read_time += (clock() - start_time) * 1000000 / CLOCKS_PER_SEC;
    }

    *mcs = read_time;
    return msg;
}
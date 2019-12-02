#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include "tasks.h"

#include "per_task.h"
#include "per_type.h"
#include "thread_pool.h"

typedef enum {
    PER_TASK,
    PER_TYPE,
    THREAD_POOL
} strategy_t;

static strategy_t strategy = -1;


static const struct option longopts[] = {
    { "strategy",       required_argument, NULL, 's' },
    { "help",           no_argument,       NULL, 'h' },
    { "thread_count",   required_argument, NULL, 'n' }
};

void show_usage()
{
    puts("Использование: processor [-h | --help] (-s | --strategy) strategy");
    puts("    -h | --help - вывести помощь");
    puts("    -s | strategy - задаёт стратегию работы");
}

bool parse_args(int argc, char** argv)
{
    if (argc < 1) return false;

    char opt = getopt_long(argc, argv, "hs:n:", longopts, NULL);
    while (opt != -1)
    {
        switch(opt)
        {
            case 'h':
                show_usage();
                exit(EXIT_SUCCESS);
            case 's':
                if (strcmp(optarg, "per_task") == 0) strategy = PER_TASK;
                else if (strcmp(optarg, "per_type") == 0) strategy = PER_TYPE;
                else if (strcmp(optarg, "thread_pool") == 0) strategy = THREAD_POOL;
                else return false;
                break;
            case 'n':
                //TODO
            default:
                show_usage();
                return false;
        }

        opt = getopt_long(argc, argv, "s:h", longopts, NULL);
    }

    return strategy != -1;
}


int main(int argc, char** argv)
{
    puts("");

    bool args_ok = parse_args(argc, argv);
    if (!args_ok)
    {
        puts("Ошибка: некорректные аргументы командной строки.");
        show_usage();
        exit(EXIT_FAILURE);
    }

    pthread_t reader;
    switch(strategy)
    {
        case PER_TASK:  pthread_create(&reader, NULL, reader_per_task, NULL);   break;
        case PER_TYPE:
            //reader_func = reader_per_type;
            break;
        case THREAD_POOL:
            //reader_func = reader_thread_pool;
            break;
    }
    
    pthread_create(&writer, NULL, writer_func, NULL);
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
}
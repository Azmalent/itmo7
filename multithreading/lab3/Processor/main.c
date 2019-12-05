#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include "include/per_task.h"
#include "include/per_type.h"
#include "include/thread_pool.h"
#include "include/types.h"
#include "include/writer.h"

#define SHORTOPTS "hs:t:"

static strategy_t strategy = PER_TASK;
static int thread_count = 5;

static const struct option LONGOPTS[] = {
    { "strategy",       required_argument, NULL, 's' },
    { "help",           no_argument,       NULL, 'h' },
    { "thread_count",   required_argument, NULL, 't' }
};

void show_usage()
{
    puts("Использование: processor "
        "[-h | --help] "
        "[(-s | --strategy) S] "
        "[(-t | --thread_count) T]"
        "[-n N]"); //TODO
    puts("    -h | --help - вывести помощь");
    puts("    -s | --strategy - стратегия работы. По умолчанию PER_TASK");
    puts("    -t | --thread_count - количество потоков в пуле для стратегии THREAD_POOL. По умолчанию 5");
    puts("    -n - "); //TODO
}

bool parse_args(int argc, char** argv)
{
    if (argc < 1) return false;

    char opt = getopt_long(argc, argv, SHORTOPTS, LONGOPTS, NULL);
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
            case 't':
                thread_count = atoi(optarg);
                if (thread_count < 1 || thread_count > MAX_THREAD_POOL_SIZE) return false;
                break;
            default:
                return false;
        }

        opt = getopt_long(argc, argv, SHORTOPTS, LONGOPTS, NULL);
    }

    return true;
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

    pthread_t reader, writer;
    switch(strategy)
    {
        case PER_TASK:  
            pthread_create(&reader, NULL, reader_per_task, NULL);   
            break;
        case PER_TYPE:  
            pthread_create(&reader, NULL, reader_per_type, NULL);   
            break;
        case THREAD_POOL:
            init_thread_pool(thread_count);
            pthread_create(&reader, NULL, reader_thread_pool, NULL);
            break;
    }
    
    pthread_create(&writer, NULL, writer_thread_func, NULL);
    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
}
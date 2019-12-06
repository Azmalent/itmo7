#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#include "include/metrics.h"
#include "include/per_task.h"
#include "include/per_type.h"
#include "include/thread_pool.h"
#include "include/types.h"
#include "include/writer.h"

#define DEFAULT_THREAD_COUNT 5

#define SHORTOPTS "hs:t:n:"

static strategy_t strategy = PER_TASK;
static int thread_count = 5;
static int reporting_interval = 0;

static const struct option LONGOPTS[] = {
    { "help",           no_argument,       NULL, 'h' },
    { "strategy",       required_argument, NULL, 's' },
    { "thread_count",   required_argument, NULL, 't' },
    { "interval",       required_argument, NULL, 'n' },
    { NULL, 0, NULL, '\0' }
};

void show_usage()
{
    puts("Использование: processor "
        "[-h | --help] "
        "[(-s | --strategy) <стратегия>] "
        "[(-t | --thread_count) <число>] "
        "[(-n | --interval) <число>]");
    puts("    -h | --help           вывести помощь");
    puts("    -s | --strategy       стратегия работы. По умолчанию PER_TASK.");
    printf("    -t | --thread_count   количество потоков в пуле для стратегии THREAD_POOL. "
        "По умолчанию %i. Максимум - %i.\n", DEFAULT_THREAD_COUNT, MAX_THREAD_POOL_SIZE);
    puts("    -n | --interval       интервал для сбора метрики в мс. "
        "По умолчанию 0 (метрика отключена).");
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
            case 'n':
                reporting_interval = atoi(optarg);
                if (reporting_interval < 0) return false;
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

    opterr = false;
    bool args_ok = parse_args(argc, argv);
    if (!args_ok)
    {
        puts("Ошибка: некорректные аргументы командной строки.");
        show_usage();
        exit(EXIT_FAILURE);
    }

    bool do_metrics = reporting_interval > 0;
    
    pthread_t reader, writer, monitor;
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
    if (do_metrics) 
    {
        init_monitor(reporting_interval, strategy);
        pthread_create(&monitor, NULL, monitor_thread_func, NULL);
    }

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);
    if (do_metrics) pthread_join(monitor, NULL);

    switch(strategy)
    {
        case PER_TASK:      finalize_per_task();    break;
        case PER_TYPE:      finalize_per_type();    break;
        case THREAD_POOL:   finalize_thread_pool(); break;
    }
}
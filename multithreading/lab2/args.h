#ifndef LAB2_ARGS
#define LAB2_ARGS

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FLAG = 0,
    NOT_FLAG = 1,
    SHOULD_TRANSFORM = 2
} arg_type_t;

arg_type_t arg_type(char* arg)
{
    if (arg[0] != '-') return NOT_FLAG;

    for (char* c = arg + 1; *c != '\0'; c++)
    {
        if (strchr("rwxXstugoa,+-=01234567", *c) != NULL) return SHOULD_TRANSFORM;    
    }

    return FLAG;
}

bool* transform_args(int argc, char* argv[])
{
    bool* marks = malloc( (argc - 1) * sizeof(bool) );
    int num_flags = 0;

    for (int i = 1; i < argc; i++)
    {
        arg_type_t type = arg_type(argv[i]);
        if (type == FLAG) num_flags++;
        else
        {
            int arg_index = i - 1 - num_flags;
            marks[arg_index] = type == SHOULD_TRANSFORM;
            if (marks[arg_index]) argv[i][0] = '_';
        }
    }

    marks = realloc( marks, (argc - 1 - num_flags) * sizeof(bool) );
    return marks;
}

void restore_args(int argc, char* argv[], bool* marks)
{
    for (int i = optind; i < argc; i++)
    {
        if (marks[i-optind]) argv[i][0] = '-';
    }
}

#endif
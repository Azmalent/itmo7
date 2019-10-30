#ifndef LAB2_IO
#define LAB2_IO

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

static char* program_name;
static locale_t locale;

ssize_t echo(int file, char* string)
{
    return write(file, string, strlen(string));
}

ssize_t echof(int file, char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char buf[4096];
    int result = vsprintf(buf, format, argptr);
    return write(file, buf, strlen(buf));
}

void echoerr(char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    char buf[4096];
    int result = vsprintf(buf, format, argptr);

    echof(STDERR_FILENO, "%s: ", program_name);
    echo(STDERR_FILENO, buf);
    if (errno) echof(STDERR_FILENO, ": %s\n", strerror_l(errno, locale));
}

#endif
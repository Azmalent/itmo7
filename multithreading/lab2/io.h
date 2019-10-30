#ifndef LAB2_IO
#define LAB2_IO

#include <locale.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static locale_t locale;

void init_locale()
{
    locale = newlocale(LC_CTYPE_MASK | LC_MESSAGES_MASK, "", 0);
}

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

ssize_t echoerr(char* string)
{
    echo(STDERR_FILENO, string);
    echo(STDERR_FILENO, ": ");
    echo(STDERR_FILENO, strerror_l(errno, locale));
    echo(STDERR_FILENO, "\n");
}

#endif
//Программа должна осуществлять весь ввод-вывод через семейства read(2) и write(2).
//Продумать обработку ошибок, используя заголовочный файл errno.h 

// Для получения возможности защиты на 4 балла:
// + программа должна уметь работать с несколькими файлами

// Для получения возможности защиты на 5 баллов:
// + программа должна обрабатывать несколько ключей оригинальной команды. 

#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "io.h"
#include "types.h"
#include "utils.h"

/* ----------------------- */
/* ---Считывание-режима--- */
/* ----------------------- */
mode_t update_perms(mode_t old, operation_t op, perms_t perms, int shift)
{
    switch (op)
    {
        case GRANT_PERMS:   return old | (perms << (shift*3));
        case REVOKE_PERMS:  return old & ~(perms << (shift*3));
        case SET_PERMS:     return old & ~(7 << (shift*3)) | (perms << (shift*3));
        default:            return old;
    }
}

bool parse_sym_action(char* str, mode_t old, class_t* class, operation_t* op, perms_t* perms)
{
    *class = 0;
    *perms = 0;
    *op = 0;

    char* s = str;

    //Пользователи
    while (1)
    {
        switch (*s)
        {
            case 'u': *class |= OWNER;    break;
            case 'g': *class |= GROUP;    break;
            case 'o': *class |= OTHERS;   break;
            case 'a': *class = OWNER | GROUP | OTHERS;  break;
            default: goto users_end; 
        }
        s++;
    }
    users_end: if (*class == 0) *class = DEFAULT;

    //Действие
    switch (*s) 
    {
        case '+': *op = GRANT_PERMS;  break;
        case '-': *op = REVOKE_PERMS; break;
        case '=': *op = SET_PERMS;    break;
        default: return FALSE;
    }
    s++;

    //Права
    //Можно задать одним символом u, g или o
    if ((*s == 'u' || *s == 'g' || *s == 'o') && *(s + 1) == '\0')
    {
        switch (*s)
        {
            case 'u': *perms = (old >> 6) & 7; break;
            case 'g': *perms = (old >> 3) & 7; break;
            case 'o': *perms = old & 7;        break;
        }

        return TRUE;
    }

    //Либо символами r, w, x
    while (*s != '\0')
    {
        switch (*s)
        {
            case 'r': *perms |= READ;          break;
            case 'w': *perms |= WRITE;         break;
            case 'x': *perms |= EXECUTE;       break;
            default: return FALSE;
        }
        s++;
    }

    return *perms != 0;
}

bool parse_sym_mode(char* str, mode_t old, mode_t* new)
{
    int numwords;
    char** words = split(str, ',', &numwords);

    mode_t mode = old;
    
    for (int i = 0; i < numwords; i++)
    {
        class_t class;
        operation_t op;
        perms_t perms;

        bool success = parse_sym_action(words[i], mode, &class, &op, &perms);
        if(!success)
        {
            for (i = i+1; i < numwords; i++) free(words[i]);
            free(words);
            return FALSE;
        }

        free(words[i]);
        if (class & OWNER)  mode = update_perms(mode, op, perms, 2);
        if (class & GROUP)  mode = update_perms(mode, op, perms, 1);
        if (class & OTHERS) mode = update_perms(mode, op, perms, 0);
        else if (class == DEFAULT) mode = update_perms(mode, op, perms & ~WRITE, 0);
    }

    free(words);
    *new = mode;
    return TRUE;
} 

bool parse_mode(char* str, mode_t old, mode_t* new)
{
    if (strlen(str) <= 3 && isOctal(str))
    {
        *new = strtol(str, NULL, 8);
        return TRUE;
    }

    return parse_sym_mode(str, old, new);
}

/* ----------------------- */
/* ----Основная-логика---- */
/* ----------------------- */
void show_usage()
{
    echo(STDOUT_FILENO, "Использование: chmod [ключи] режим файл1 [файл2 ...]\n\n");

    echo(STDOUT_FILENO, "Ключи:\n");
    echo(STDOUT_FILENO, "\t-R: рекурсивное изменение прав для директорий и их содержимого\n");
    echo(STDOUT_FILENO, "\t-f: не выводить сообщения об ошибке\n");
    echo(STDOUT_FILENO, "\t-v: подробно описывать действие для каждого файла\n");
    echo(STDOUT_FILENO, "\t-c: подробно описывать действие только при наличии изменений\n");
}

bool parse_flags(int argc, char* argv[], flags_t* flags)
{
    *flags = 0;

    char opt = getopt(argc, argv, "Rfvc");
    while(opt != -1)  
    {  
        switch(opt)  
        {  
            case 'R': *flags |= RECURSIVE;      break; 
            case 'f': *flags |= NO_ERRORS;      break;
            case 'v': *flags |= VERBOSE;        break;
            case 'c': *flags |= CHANGES_ONLY;   break;
            case '?': return FALSE;
        }  

        opt = getopt(argc, argv, "Rfv");
    }

    return TRUE;
}

char* mode_str(mode_t mode)
{  
    char* buf = malloc(10 * sizeof(char));
    for (int i = 0; i < 3; i++)
    {
        buf[i*3 + 0] = (mode & (1 << (8 - i*3))) ? 'r' : '-';
        buf[i*3 + 1] = (mode & (1 << (7 - i*3))) ? 'w' : '-';
        buf[i*3 + 2] = (mode & (1 << (6 - i*3))) ? 'x' : '-';
    }
    buf[9] = '\0';
    return buf;
}

int main(int argc, char* argv[])
{
    opterr = FALSE;
    
    flags_t flags;
    bool success = parse_flags(argc, argv, &flags);
    if (!success)
    {
        echof(STDERR_FILENO, "Неверный ключ: -%c.\n\n", optopt);
        show_usage();
        return EXIT_FAILURE;
    }

    int numfiles = argc - optind - 1;
    if (numfiles < 1)
    {
        show_usage();
        return EXIT_FAILURE;
    }

    char** files = argv + (argc - numfiles);

    int error_code = EXIT_SUCCESS;
    for (int i = 0; i < numfiles; i++)
    {
        struct stat fstat;
        if(stat(files[i], &fstat) < 0)  
        {
            if ( !(flags & NO_ERRORS) ) echoerr(files[i]);
            error_code = EXIT_FAILURE;
            continue;
        }

        mode_t old = fstat.st_mode & 0777;
        mode_t new;

        success = parse_mode(argv[optind], old, &new);
        if (!success)
        {
            echof(STDERR_FILENO, "Неверный режим: '%s'.", argv[optind]);
            show_usage();
            return EXIT_FAILURE;
        }

        if (chmod(files[i], new) != 0)
        {
            if ( !(flags & NO_ERRORS) ) echoerr(files[i]);
            error_code = EXIT_FAILURE;
            continue;
        }

        if ( (flags & RECURSIVE) && S_ISDIR(fstat.st_mode) ) 
        {
            //todo: recursive
        }
        
        if ( (flags & VERBOSE) && old == new) 
        {
            char* old_str = mode_str(old);

            echof(STDOUT_FILENO, "%s: права доступа оставлены неизменными в виде %#04o (%s)\n", 
                files[i], old, old_str);
            
            free(old_str);
        }
        else if ( (flags & VERBOSE || flags & CHANGES_ONLY) && old != new )
        {
            char* old_str = mode_str(old);
            char* new_str = mode_str(new);
            
            echof(STDOUT_FILENO, "%s: права доступа изменены с %#04o (%s) на %#04o (%s)\n", 
                files[i], old, old_str, new, new_str);
            
            free(old_str);
            free(new_str);
        }
    }

    return error_code;
}
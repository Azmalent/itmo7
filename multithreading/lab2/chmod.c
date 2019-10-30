//Программа должна осуществлять весь ввод-вывод через семейства read(2) и write(2).
//Продумать обработку ошибок, используя заголовочный файл errno.h 

// Для получения возможности защиты на 4 балла:
// + программа должна уметь работать с несколькими файлами

// Для получения возможности защиты на 5 баллов:
// + программа должна обрабатывать несколько ключей оригинальной команды. 

#include <dirent.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "echo.h"
#include "types.h"
#include "utils.h"
#include "args.h"

static int exit_code = EXIT_SUCCESS;
static char* current_file = NULL;

void mode_str(char* buf, mode_t mode)
{  
    for (int i = 0; i < 3; i++)
    {
        buf[i*3 + 0] = (mode & (1 << (8 - i*3))) ? 'r' : '-';
        buf[i*3 + 1] = (mode & (1 << (7 - i*3))) ? 'w' : '-';
        buf[i*3 + 2] = (mode & (1 << (6 - i*3))) ? 'x' : '-';
    }
    if (mode & 04000) buf[2] = (buf[2] == 'x') ? 's' : 'S';
    if (mode & 02000) buf[5] = (buf[5] == 'x') ? 's' : 'S';
    if (mode & 01000) buf[8] = (buf[8] == 'x') ? 't' : 'T';

    buf[9] = '\0';
}

/* ----------------------- */
/* ---Считывание-режима--- */
/* ----------------------- */
mode_t get_perm_changes(class_t class, perms_t perms, bool use_umask)
{
    mode_t changes = 0;
    perms_t normal_perms = perms & 7;

    if (class & OWNER) 
    {
        changes |= (normal_perms << 6);
        if (perms & SET_ID) changes |= S_ISUID;
    }

    if (class & GROUP) 
    {
        changes |= (normal_perms << 3);
        if (perms & SET_ID) changes |= S_ISGID;
    }

    if (class & OTHERS) 
    {
        changes |= normal_perms;
        if (perms & STICKY) changes |= 01000;
    }

    if (use_umask) changes &= ~umask(0);

    return changes;
}

mode_t update_perms(mode_t old, class_t class, operation_t op, perms_t perms)
{
    mode_t mode = old;

    bool use_umask = class == DEFAULT;
    if (use_umask) class = EVERYONE;

    //+X равно +x для директорий или когда у кого-нибудь уже есть +x
    if ( (perms & SPECIAL_EXECUTE) && (S_ISDIR(old) || old & 0111) ) perms |= EXECUTE;

    if (op == SET_PERMS)
    {
        if (class & OWNER) mode &= ~(7 << 6);
        if (class & GROUP) mode &= ~(7 << 3);
        if (class & OTHERS) mode &= ~7;
    }

    mode_t changes = get_perm_changes(class, perms, use_umask);

    if (op == REVOKE_PERMS) mode &= ~changes;
    else mode |= changes;
    
    return mode;
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
            case 'a': *class = EVERYONE;  break;
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
        default: return false;
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

        return true;
    }

    //...либо символами r, w, x
    while (*s != '\0')
    {
        switch (*s)
        {
            case 'r': *perms |= READ;       break;
            case 'w': *perms |= WRITE;      break;
            case 'x': *perms |= EXECUTE;    break;
            case 'X': *perms |= SPECIAL_EXECUTE;    break;
            case 's': *perms |= SET_ID;     break;
            case 't': *perms |= STICKY;     break;
            default: return false;
        }
        s++;
    }

    return *perms != 0;
}

bool parse_sym_mode(char* str, mode_t old, mode_t* new)
{
    int num_words;
    char** words = split(str, ',', &num_words);

    mode_t mode = old;

    for (int i = 0; i < num_words; i++)
    {
        class_t class;
        operation_t op;
        perms_t perms;

        char* word = words[i];

        //Особый случай - [+-=]число
        if (strchr("+-=", word[0]) != NULL && is_numeric_mode(word + 1))
        {
            mode_t mode_change = strtol(word + 1, NULL, 8);
            switch (word[0])
            {
                case '+': mode |= mode_change;  break;
                case '-': mode &= ~mode_change; break;
                case '=': mode = mode_change;   break;
            }
        }
        else 
        {
            bool success = parse_sym_action(word, mode, &class, &op, &perms);
            if(!success)
            {
                for (i = i+1; i < num_words; i++) free(word);
                free(words);
                return false;
            }
            mode = update_perms(mode, class, op, perms);
        }

        free(word);
    }

    free(words);
    *new = mode | (old & ~07777);
    return true;
} 

bool parse_mode(char* str, mode_t old, mode_t* new)
{
    if (is_numeric_mode(str))
    {
        *new = strtol(str, NULL, 8);
        if (S_ISDIR(old)) *new |= old & ~01777;
        return true;
    }

    return parse_sym_mode(str, old, new);
}

/* ----------------------- */
/* ----Основная-логика---- */
/* ----------------------- */
void show_usage()
{
    echo(STDOUT_FILENO, "\nИспользование: chmod [ключи] режим файл1 [файл2 ...]\n\n");

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
            case '?': return false;
        }  

        opt = getopt(argc, argv, "Rfvc");
    }

    return true;
}

int process_file(char* file, char* mode_arg, flags_t flags, bool recursed)
{
    struct stat lfstat, fstat;
    if(lstat(file, &lfstat) < 0 || stat(file, &fstat) < 0)  
    {
        if ( !(flags & NO_ERRORS) ) echoerr("невозможно получить доступ к '%s'", file);
        if (flags & VERBOSE) echof(STDOUT_FILENO, "не удалось изменить права доступа '%s' с 0000 (---------) на 0000 (---------)", file);
        return EXIT_FAILURE;
    }

    if (S_ISLNK(lfstat.st_mode) && recursed)
    {
        if (flags & VERBOSE) echof(STDOUT_FILENO, "ни символьная ссылка '%s', ни то но что она указывает не были изменены\n", file);
        return EXIT_SUCCESS;
    }

    mode_t old = fstat.st_mode;
    mode_t new;

    bool success = parse_mode(mode_arg, old, &new);
    if (!success)
    {
        echoerr("неверный режим: «%s»", mode_arg);
        show_usage();
        return -1;
    }

    char old_str[10], new_str[10]; 
    mode_str(old_str, old);
    mode_str(new_str, new);

    if (chmod(file, new) != 0)
    {              
        echof(STDOUT_FILENO, "не удалось изменить права доступа '%s' с %4.4o (%s) на %4.4o (%s)\n", 
            file, old & 07777, old_str, new & 07777, new_str);

        return EXIT_FAILURE;
    }
    
    bool mode_changed = (old ^ new) & 07777;
    if (mode_changed)
    {
        if (flags & (VERBOSE | CHANGES_ONLY))
        {
            echof(STDOUT_FILENO, "права доступа '%s' изменены с %4.4o (%s) на %4.4o (%s)\n", 
                file, old & 07777, old_str, new & 07777, new_str);
        }
    }
    else if (flags & VERBOSE)
    {
        echof(STDOUT_FILENO, "права доступа '%s' оставлены в виде %4.4o (%s)\n", file, old & 07777, old_str);
    }

    if ( (flags & RECURSIVE) && S_ISDIR(old) ) 
    {
        DIR* dir = opendir(file);
        if (dir != NULL)
        {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

                char* relative_path = get_relative_path(file, entry->d_name);
                exit_code |= process_file(relative_path, mode_arg, flags, true);
                free(relative_path);
            }

            closedir(dir);
            return exit_code;
        }
        else
        {
            if ( !(flags & NO_ERRORS) ) echoerr("невозможно прочитать каталог '%s'", file);
            if ( flags & VERBOSE ) 
            {
                echof(STDOUT_FILENO, "не удалось изменить права доступа '%s' с %4.4o (%s) на %4.4o (%s)\n", 
                    file, old & 07777, old_str, new & 07777, new_str);
            }

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    program_name = argv[0];
    locale = newlocale(LC_CTYPE_MASK | LC_MESSAGES_MASK, "", 0);

    opterr = false;
    
    bool* marks = transform_args(argc, argv);

    flags_t flags;
    bool success = parse_flags(argc, argv, &flags);
    if (!success)
    {
        echoerr("неверный ключ — «%c»", optopt);
        show_usage();
        return EXIT_FAILURE;
    }

    int num_files = argc - optind - 1;
    if (num_files < 1)
    {
        show_usage();
        return EXIT_FAILURE;
    }

    restore_args(argc, argv, marks);
    free(marks);

    char* mode = argv[optind];
    char** files = argv + (argc - num_files);

    for (int i = 0; i < num_files; i++) 
    {
        current_file = files[i];
        int result = process_file(current_file, mode, flags, false);
        if (result == -1) break;
        exit_code |= result;
    }

    return exit_code;
}
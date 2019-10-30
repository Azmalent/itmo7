#ifndef LAB2_UTILS
#define LAB2_UTILS

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

//Считает количество вхождений символа в строку
int count_occurences(char* str, char c)
{
    int n = 0;
    for (char* s = str; *s != '\0'; s++)
    {
        if (*s == c) n++;
    }

    return n;
}

//Возвращает подстроку между двумя указателями
char* substring(char* start, char* end)
{
    int length = end - start;
    char* substr = malloc( sizeof(char) * (length + 1) );
    memcpy(substr, start, length + 1);
    substr[length] = '\0';

    return substr;
}

//Разбивает строку по заданному символу
char** split(char* str, char delim, int* n)
{
    int numWords = count_occurences(str, delim) + 1;
    char** words = malloc( sizeof(char*) * numWords );
    
    char* start = str;
    char* end = start;

    int i = 0;
    while(*end != '\0')
    {
        while (*end != delim && *end != '\0') end++;
        words[i] = substring(start, end);
        i++;

        if (*end == delim) 
        {
            end++;
            start = end;
        }
    }

    *n = numWords;
    return words;
}

//Возвращает true, если строка полностью состоит из 1-4 цифр от 0 до 7
bool is_numeric_mode(char* str)
{
    if (*str == '\0') return false;

    char* s = str;
    while(*s == '0') s++;
    int len = strlen(s);
    if (len > 4) return false;

    while (*s != '\0')
    {
        if (*s < '0' || *s > '7') return false;
        s++;
    }

    return true;
}

//Возвращает относительный путь к файлу
char* get_relative_path(char* dir, char* file)
{
    int dirlen = strlen(dir);
    int filelen = strlen(file);

    char* relative_path = malloc( (dirlen + filelen + 2) * sizeof(char) );

    memcpy(relative_path, dir, dirlen);
    relative_path[dirlen] = '/';
    memcpy(relative_path + dirlen + 1, file, filelen + 1);

    return relative_path;
}

#endif
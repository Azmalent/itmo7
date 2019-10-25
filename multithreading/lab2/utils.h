#ifndef LAB2_UTILS
#define LAB2_UTILS

//Считает количество вхождений символа в строку
int countOccurences(char* str, char c)
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
    int numWords = countOccurences(str, delim) + 1;
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

//Возвращает true, если строка полностью состоит из цифр от 0 до 7
bool isOctal(char* str)
{
    for (char* s = str; *s != '\0'; s++)
    {
        if (*s < '0' || *s > '7') return FALSE;
    }

    return TRUE;
}

#endif
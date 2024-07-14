#include <stdlib.h>
#include <string.h>
#include "remove-quotes.h"

char *removeQuotes(const char *str)
{
    int length = strlen(str);
    char *result = malloc(length + 1); // Allocate memory for the new string
    char *ptr = result;                // Pointer to iterate over the new string

    for (int i = 0; i < length; i++)
    {
        if (str[i] != '"') // Skip quotes
        {
            *ptr++ = str[i];
        }
    }
    *ptr = '\0'; // add \0 at the end of string

    return result;
}

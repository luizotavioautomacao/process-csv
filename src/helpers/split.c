#include <stdlib.h>
#include <string.h>
#include "split.h"
#include "remove-quotes.h"

char **split(const char *str, char delimiter, int *count)
{
    // Check if the string is empty or NULL
    if (str == NULL || *str == '\0')
    {
        *count = 0;
        return NULL;
    }

    char **result = 0;
    size_t count_temp = 0;
    const char *tmp = str;
    const char *last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    // Count how many elements will be extracted
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count_temp++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for the last element
    count_temp += last_comma < (str + strlen(str) - 1);

    // Add space for trailing token
    count_temp++;

    result = malloc(sizeof(char *) * count_temp);

    if (result)
    {
        size_t idx = 0;
        char *str_copy = strdup(str); // Make a copy of the original string
        if (!str_copy)
        {
            // Handle memory allocation failure
            *count = 0;
            return NULL;
        }

        char *token = strtok(str_copy, delim);

        while (token)
        {
            // Ensure that we don't exceed the buffer size
            if (idx < count_temp)
            {
                *(result + idx++) = removeQuotes(token); // Use strdup to copy token
            }
            token = strtok(0, delim);
        }
        *(result + idx) = 0;

        free(str_copy); // Free the temporary copy of the string
    }

    *count = count_temp - 1;

    return result;
}
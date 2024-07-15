#include <string.h>
#include "is-array-duplicate.h"

int isArrayDuplicate(char **array, int count, const char *value)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(array[i], value) == 0)
        {
            return 1; // duplicate value found
        }
    }
    return 0; // no value found
}

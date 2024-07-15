#include <stdlib.h>
#include "free-string-array.h"

void freeStringArray(char **array)
{
    if (array == NULL)
    {
        return;
    }

    for (int i = 0; array[i] != NULL; i++)
    {
        free(array[i]);
    }
    free(array);
}

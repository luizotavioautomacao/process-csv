#include <stdio.h>
#include <stdlib.h>
#include "handle-error.h"

void handleError(const char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE); // ou return um valor de erro, se apropriado
}

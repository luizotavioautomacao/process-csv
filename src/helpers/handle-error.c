#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 
#include "handle-error.h"

void handleError(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
    exit(EXIT_FAILURE); // close the program
}
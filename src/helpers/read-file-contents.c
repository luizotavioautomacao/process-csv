#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include "read-file-contents.h"

int READ_FILE_CONTENTS_DEBUG_LOG = 0;

char *readFileContents(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Failed to open file");
        return NULL;
    }

    // Calculate the number of lines
    int line_count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line_count++;
        }
    }

    if (READ_FILE_CONTENTS_DEBUG_LOG == 1)
        printf("número de linhas: %d\n", line_count);

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        fclose(file);
        perror("Failed to allocate memory for file content");
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

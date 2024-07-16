#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read-file-contents.h"

extern int DEBUG_LOG;

char *readFileContents(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        const char *prefix = "../../";
        char *fullPath = malloc(strlen(prefix) + strlen(filePath) + 1);
        strcpy(fullPath, prefix);   // Copy prefix to fullPath
        strcat(fullPath, filePath); // Concatenates filePath into fullPath
        
        file = fopen(fullPath, "r");

        if (!file)
        {
            perror("Failed to open file");
            return NULL;
        }
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

    if (DEBUG_LOG == 1)
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

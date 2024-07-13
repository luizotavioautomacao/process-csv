#include <stdio.h>  // printf [manipular entradas e saídas]
#include <stdlib.h> // malloc, realloc [alocação dinâmica de memória]
#include <string.h> // strdup, strtok [manipulação de strings]
#include "libcsv.h"

// Function to remove quotes from a string
char *removeQuotes(const char *str) {
    printf("quotes: %s\n", str);
    int length = strlen(str);
    char *result = malloc(length + 1);  // Allocate memory for the new string
    char *ptr = result;  // Pointer to iterate over the new string

    for (int i = 0; i < length; i++) {
        if (str[i] != '"') {  // Skip quotes
            *ptr++ = str[i];
        }
    }
    *ptr = '\0';  // Null-terminate the new string

    return result;
}

// Helper function to split a string by a delimiter
char **split(const char *str, char delimiter, int *count)
{
    char *temp = strdup(str);
    int num_tokens = 0;
    char **tokens = NULL;
    char *token = strtok(temp, &delimiter);
    while (token != NULL)
    {
        tokens = realloc(tokens, sizeof(char *) * (++num_tokens));
        tokens[num_tokens - 1] = removeQuotes(token);
        token = strtok(NULL, &delimiter);
    }
    tokens = realloc(tokens, sizeof(char *) * (num_tokens + 1));
    tokens[num_tokens] = NULL;
    *count = num_tokens;
    free(temp);
    return tokens;
}

// Helper function to free the memory allocated for a Matrix of string
void freeMatrixMemory(char **matrix)
{
    for (int i = 0; matrix[i] != NULL; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// Process CSV data given as a string
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    // Imprimindo os parâmetros
    printf("%s\n", csv);
    printf("Selected Columns: %s\n", selectedColumns);
    printf("Filter: %s\n\n", rowFilterDefinitions);
    fflush(stdout); // Garante que a saída é imediatamente escrita no stdout

    int line_count = 0;
    char **lines = split(csv, '\n', &line_count);

    if (line_count <= 0)
    {
        freeMatrixMemory(lines);
        return;
    }

    for (int i = 0; i < line_count; i++)
    {
        printf("line%d: %s\n", i + 1, lines[i]);
    }

    int column_count = 0;
    char **headers = split(lines[0], ',', &column_count);

    for (int i = 0; i < column_count; i++)
    {
        printf("headers%d: %s\n", i + 1, headers[i]);
    }
}

// Process CSV data from a file
void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    // Imprimindo os parâmetros
    printf("CSV File Path: %s\n", csvFilePath);
    printf("Selected Columns: %s\n", selectedColumns);
    printf("Row Filter Definitions: %s\n", rowFilterDefinitions);
    fflush(stdout); // Garante que a saída seja imediatamente escrita no stdout
}

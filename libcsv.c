#include <stdio.h>  // printf [manipular entradas e saídas]
#include <stdlib.h> // malloc, realloc [alocação dinâmica de memória]
#include <string.h> // strdup, strtok [manipulação de strings]
#include "libcsv.h"

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
        tokens[num_tokens - 1] = strdup(token);
        token = strtok(NULL, &delimiter);
    }
    tokens = realloc(tokens, sizeof(char *) * (num_tokens + 1));
    tokens[num_tokens] = NULL;
    *count = num_tokens;
    free(temp);
    return tokens;
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

    for (int i = 0; i < line_count; i++) {
        printf("Line %d: %s\n", i + 1, lines[i]);
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

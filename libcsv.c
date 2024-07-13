#include <stdio.h>  // printf [manipular entradas e saídas]
#include <stdlib.h> // malloc, realloc [alocação dinâmica de memória]
#include <string.h> // strdup, strtok [manipulação de strings]
#include "libcsv.h"

// Function to remove quotes from a string
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
    *ptr = '\0'; // add \o the end of string

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
    printf("\n");

    for (int i = 0; i < column_count; i++)
    {
        printf("headers%d: %s\n", i + 1, headers[i]);
    }

    // Determine which columns to select
    int selected_count = 0;
    char **selected = split(selectedColumns, ',', &selected_count);
    int *selected_indices = malloc(sizeof(int) * selected_count);

    for (int i = 0; i < selected_count; i++)
    {
        printf("\ncolumn selected%d: %s ", i + 1, selected[i]);
        // printf("\nselected_indices: %d\n", selected_indices[i]);
    }

    printf("\n");
    if (selected_count > 0)
    {
        for (int i = 0; i < selected_count; i++)
        {
            for (int j = 0; j < column_count; j++)
            {
                if (strcmp(selected[i], headers[j]) == 0)
                {
                    printf("\nselected: %s", selected[i]);
                    printf("\nheaders: %s", headers[j]);
                    printf("\ni: %d", i);
                    printf("\nj: %d", j);
                    selected_indices[i] = j;
                    break;
                }
            }
        }
    }

    // If selected column is equal 0, then select all
    if (selected_count == 0)
    {
        selected_indices = realloc(selected_indices, sizeof(int) * column_count);
        selected_count = column_count;
        for (int i = 0; i < column_count; i++)
        {
            selected_indices[i] = i;
        }
    }

    for (int i = 0; i < selected_count; i++)
    {
        printf("column selected: %d\n", selected_indices[i]);
    }

    // Process row filters
    int filter_count = 0;
    char **filters = split(rowFilterDefinitions, '\n', &filter_count);
    int *filter_indices = malloc(sizeof(int) * filter_count);
    char **filter_operators = malloc(sizeof(char *) * filter_count);
    char **filter_values = malloc(sizeof(char *) * filter_count);

    for (int i = 0; i < filter_count; i++)
    {
        printf("\nfilters[i]: %s", filters[i]);
        char *filter = strdup(filters[i]);
        printf("\nfilter: %s", filter);

        // Find operator
        char *operator= NULL;
        if (strstr(filter, ">="))
            operator= ">=";
        else if (strstr(filter, "<="))
            operator= "<=";
        else if (strstr(filter, ">"))
            operator= ">";
        else if (strstr(filter, "<"))
            operator= "<";
        else if (strstr(filter, "="))
            operator= "=";

        char *header = strtok(filter, "><=");
        char *value = strtok(NULL, "><=");

        for (int j = 0; j < column_count; j++)
        {
            if (strcmp(header, headers[j]) == 0)
            {
                filter_indices[i] = j;
                filter_operators[i] = strdup(operator);
                filter_values[i] = strdup(value);
                printf("\nheader: %s", header);
                printf("\noperator: %s", operator);
                printf("\nvalue: %s\n", value);
                break;
            }
        }
        free(filter);
    }

    for (int i = 0; i < filter_count; i++)
    {
        printf("\nfilter_indices: %d", filter_indices[i]);
        printf("\nfilter_operators: %s", filter_operators[i]);
        printf("\nfilter_values: %s", filter_values[i]);
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

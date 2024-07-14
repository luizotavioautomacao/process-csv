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

    if (column_count > 256)
    {
        fputs("This cvs can have a maximum of 256 columns!\n", stderr);
        return;
    }

    for (int i = 0; i < column_count; i++)
    {
        printf("headers%d: %s\n", i + 1, headers[i]);
    }

    // Determine which columns to select
    int selected_count = 0;
    char **selecteds = split(selectedColumns, ',', &selected_count);
    int *selected_indice_column = malloc(sizeof(int) * selected_count);

    printf("\nselected_count: %d \n", selected_count);
    if (selected_count > 0)
    {
        for (int i = 0; i < selected_count; i++)
        {
            for (int j = 0; j < column_count; j++)
            {
                if (strcmp(selecteds[i], headers[j]) == 0)
                {
                    selected_indice_column[i] = j;
                    printf("selecteds: %s\n", selecteds[i]);
                    break;
                }
            }
        }
    }

    // If selected column is equal 0, then select all
    if (selected_count == 0)
    {
        printf("select all");
        selected_indice_column = realloc(selected_indice_column, sizeof(int) * column_count);
        selected_count = column_count;
        for (int i = 0; i < column_count; i++)
        {
            selected_indice_column[i] = i;
        }
    }

    // Process row filters
    int filter_count = 0;
    char **filters = split(rowFilterDefinitions, '\n', &filter_count);
    int *filter_indice_column = malloc(sizeof(int) * filter_count);
    char **filter_operators = malloc(sizeof(char *) * filter_count);
    char **filter_values = malloc(sizeof(char *) * filter_count);

    printf("\n");
    for (int i = 0; i < filter_count; i++)
    {
        // printf("\nfilters[i]: %s", filters[i]);
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
                filter_indice_column[i] = j;
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
        printf("\nfilter_indice_column: %d", filter_indice_column[i]);
        printf("\nfilter_operators: %s", filter_operators[i]);
        printf("\nfilter_values: %s\n", filter_values[i]);
        printf("--#--\n\n");
    }

    // Process each row

    // headers
    for (int k = 0; k < selected_count; k++)
    {
        int column_selected = selected_indice_column[k];
        char *header = headers[column_selected];

        fputs(header, stdout);
        if (k < selected_count - 1)
        {
            fputs(",", stdout);
        }
    }

    // values
    for (int i = 0; i < line_count; i++) // don't consider the headers
    {
        int row_count = 0;
        char **row = split(lines[i], ',', &row_count);
        int match = 1;
        printf("\ni: %d", i);

        for (int j = 0; j < filter_count; j++)
        {
            int column_filter = filter_indice_column[j];
            char *value = row[column_filter];

            if (strcmp(filter_operators[j], "=") == 0)
            {
                if (strcmp(value, filter_values[j]) == 0)
                {
                    printf("\n[=] j: %d filter_indice_column: %d value: %s", j, column_filter, value);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(filter_operators[j], ">") == 0)
            {
                if (atof(value) > atof(filter_values[j]))
                {
                    printf("\n[>] j: %d filter_indice_column: %d value: %s", j, column_filter, value);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(filter_operators[j], ">=") == 0)
            {
                if (atof(value) >= atof(filter_values[j]))
                {
                    printf("\n[>=] j: %d filter_indice_column: %d value: %s", j, column_filter, value);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(filter_operators[j], "<") == 0)
            {
                if (atof(value) < atof(filter_values[j]))
                {
                    printf("\n[<] j: %d filter_indice_column: %d value: %s", j, column_filter, value);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(filter_operators[j], "<=") == 0)
            {
                if (atof(value) <= atof(filter_values[j]))
                {
                    printf("\n[<=] j: %d filter_indice_column: %d value: %s", j, column_filter, value);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
        }

        if (match)
        {
            char **result_value = malloc(selected_count * sizeof(char *));

            int result_count = 0;

            printf("\n\n");

            printf("\n");
            for (int k = 0; k < selected_count; k++)
            {
                int column_selected = selected_indice_column[k];

                char *value = row[column_selected];

                int repeat = 0;
                for (int z = 0; z < result_count; z++)
                {
                    if (result_value[z] == value)
                    {
                        repeat = 1;
                        break;
                    }
                }
                if (repeat == 0)
                {
                    result_value[result_count++] = value;
                }
            }

            for (int y = 0; y < result_count; y++)
            {
                fputs(result_value[y], stdout);
                if (y < result_count - 1)
                {
                    fputs(",", stdout);
                }
            }
            printf("\n");
        }
        freeMatrixMemory(row);
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

#include <stdio.h>  // printf [manipular entradas e saídas]
#include <stdlib.h> // malloc, realloc [alocação dinâmica de memória]
#include <string.h> // strdup, strtok [manipulação de strings]
#include "libcsv.h"
#include "src/helpers/remove-quotes.h"
#include "src/helpers/split.h"
#include "src/helpers/is-array-duplicate.h"
#include "src/helpers/free-string-array.h"
#include "src/helpers/handle-error.h"
#include "src/helpers/read-file-contents.h"

int DEBUG_LOG = 0;

// Process CSV data given as a string
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    // Printing the parameters
    if (DEBUG_LOG == 1)
    {
        printf("%s\n", csv);
        printf("Selected Columns: %s\n", selectedColumns);
        printf("Filter: %s\n\n", rowFilterDefinitions);
        fflush(stdout); // Ensures that output is immediately written to stdout
    }

    int line_count = 0;
    char **lines = split(csv, '\n', &line_count);

    if (line_count <= 0)
    {
        freeStringArray(lines);
        return;
    }

    if (DEBUG_LOG == 1)
    {
        for (int i = 0; i < line_count; i++)
        {
            printf("line%d: %s\n", i + 1, lines[i]);
        }
    }

    int column_count = 0;
    char **headers = split(lines[0], ',', &column_count);

    if (DEBUG_LOG == 1)
        printf("\ncolumn: %d\nline: %d\n", column_count, line_count);

    if (column_count > 256)
    {
        fputs("This cvs can have a maximum of 256 columns!\n", stderr);
        return;
    }

    // Verificar se há nomes duplicados nas colunas
    for (int i = 0; i < column_count; i++)
    {
        if (isArrayDuplicate(headers, i, headers[i]))
        {
            fprintf(stderr, "Duplicate column name found: %s\n", headers[i]);
            freeStringArray(headers);
            freeStringArray(lines);
            return;
        }
    }
    if (DEBUG_LOG == 1)
    {
        for (int i = 0; i < column_count; i++)
        {
            printf("headers%d: %s\n", i + 1, headers[i]);
        }
    }

    // Determine which columns to select
    int selected_count = 0;
    char **selecteds = split(selectedColumns, ',', &selected_count);
    int *selected_indice_column = malloc(sizeof(int) * selected_count);

    if (DEBUG_LOG == 1)
    {
        printf("\nselectedColumns: %s \n", selectedColumns);
        printf("\nselected_count: %d \n", selected_count);
    }

    if (selected_count > 0)
    {
        for (int i = 0; i < selected_count; i++)
        {
            for (int j = 0; j < column_count; j++)
            {
                if (strcmp(selecteds[i], headers[j]) == 0)
                {
                    selected_indice_column[i] = j;
                    if (DEBUG_LOG == 1)
                        printf("selecteds: %s\n", selecteds[i]);
                    break;
                }
            }
        }
        freeStringArray(selecteds);
    }

    // If selected column is equal 0, then select all
    if (selected_count == 0)
    {
        if (DEBUG_LOG == 1)
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

    if (DEBUG_LOG == 1)
        printf("\n");

    if (filter_count > 0)
    {
        for (int i = 0; i < filter_count; i++)
        {
            char *filter = strdup(filters[i]);

            if (DEBUG_LOG == 1)
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

            // Initialize to NULL to handle potential allocation failures
            filter_operators[i] = NULL;
            filter_values[i] = NULL;

            for (int j = 0; j < column_count; j++)
            {
                if (strcmp(header, headers[j]) == 0)
                {
                    filter_indice_column[i] = j;
                    filter_operators[i] = strdup(operator);
                    filter_values[i] = strdup(value);
                    if (filter_operators[i] == NULL || filter_values[i] == NULL)
                    {
                        free(filter_operators[i]);
                        free(filter_values[i]);
                        filter_operators[i] = NULL;
                        filter_values[i] = NULL;
                        handleError("Algo aconteceu de errado e não foi possível carregar o CSV, tente novamente!\n");
                    }

                    if (DEBUG_LOG == 1)
                    {
                        printf("\nheader: %s", header);
                        printf("\noperator: %s", operator);
                        printf("\nvalue: %s\n", value);
                    }
                    break;
                }
            }
            free(filter);
        }

        freeStringArray(filters);

        if (DEBUG_LOG == 1)
        {
            for (int i = 0; i < filter_count; i++)
            {
                printf("\nfilter_count: %d", filter_count);
                printf("\nfilter_indice_column[i]: %d", filter_indice_column[i]);
                printf("\nfilter_operators[i]: %s", filter_operators[i]);
                printf("\nfilter_values[i]: %s\n", filter_values[i]);
                printf("--#--\n\n");
            }
        }
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
    fputs("\n", stdout);

    // values
    for (int i = 0; i < line_count; i++) // don't consider the headers
    {
        int row_count = 0;
        char **row = split(lines[i], ',', &row_count);
        int match = 1;

        if (DEBUG_LOG == 1)
            printf("\ni: %d", i);

        for (int j = 0; j < filter_count; j++)
        {
            int column_filter = filter_indice_column[j];
            char *value = row[column_filter];

            if (strcmp(filter_operators[j], "=") == 0)
            {
                if (strcmp(value, filter_values[j]) == 0)
                {
                    if (DEBUG_LOG == 1)
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
                // printf("\n____________________");
                // printf("\nvalue: %s", value);
                // printf("\nfilter_values: %s", filter_values[j]);
                // printf("\n____________________\n");
                if (strcmp(value, filter_values[j]) > 0)
                {
                    if (DEBUG_LOG == 1)
                        printf("\n[>] j: %d filter_indice_column: %d value: %s filter_values[j]: %s", j, column_filter, value, filter_values[j]);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(filter_operators[j], ">=") == 0)
            {
                if (strcmp(value, filter_values[j]) >= 0)
                {
                    if (DEBUG_LOG == 1)
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
                if (strcmp(value, filter_values[j]) < 0)
                {
                    if (DEBUG_LOG == 1)
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
                if (strcmp(value, filter_values[j]) <= 0)
                {
                    if (DEBUG_LOG == 1)
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

            if (DEBUG_LOG == 1)
                printf("\n\n");

            for (int k = 0; k < selected_count; k++)
            {
                int column_selected = selected_indice_column[k];
                char *value = row[column_selected];

                int repeat = 0;
                for (int z = 0; z < result_count; z++)
                {
                    if (strcmp(result_value[z], value) == 0)
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

            free(result_value);
        }
        freeStringArray(row);
    }

    freeStringArray(headers);
    freeStringArray(filter_operators);
    // freeStringArray(filter_values); // error in big matrix ? free(): invalid pointer \n Aborted (core dumped)
    free(selected_indice_column);
    free(filter_indice_column);
}

// Process CSV data from a file
void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    char *csv = readFileContents(csvFilePath);
    if (!csv)
    {
        handleError("Failed to read CSV file.\n");
        return;
    }

    processCsv(csv, selectedColumns, rowFilterDefinitions);
    free(csv);
}

#include <stdio.h>  // printf [manipular entradas e saídas]
#include <stdlib.h> // malloc, realloc [alocação dinâmica de memória]
#include <string.h> // strdup, strtok [manipulação de strings]
#include "libcsv.h"
#include "src/helpers/remove-quotes.h"

int DEBUG_LOG = 0;
void handle_error()
{
    fprintf(stderr, "Algo aconteceu de errado e não foi possível carregar o CSV, tente novamente!\n");
    exit(EXIT_FAILURE); // ou retorne um valor de erro, se apropriado
}

char **split(const char *str, char delimiter, int *count)
{
    // Check if the string is empty or NULL
    if (str == NULL || *str == '\0')
    {
        *count = 0;
        return NULL;
    }

    char **result = 0;
    size_t count_temp = 0;
    const char *tmp = str;
    const char *last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    // Count how many elements will be extracted
    while (*tmp)
    {
        if (delimiter == *tmp)
        {
            count_temp++;
            last_comma = tmp;
        }
        tmp++;
    }

    // Add space for the last element
    count_temp += last_comma < (str + strlen(str) - 1);

    // Add space for trailing token
    count_temp++;

    result = malloc(sizeof(char *) * count_temp);

    if (result)
    {
        size_t idx = 0;
        char *str_copy = strdup(str); // Make a copy of the original string
        if (!str_copy)
        {
            // Handle memory allocation failure
            *count = 0;
            return NULL;
        }

        char *token = strtok(str_copy, delim);

        while (token)
        {
            // Ensure that we don't exceed the buffer size
            if (idx < count_temp)
            {
                *(result + idx++) = removeQuotes(token); // Use strdup to copy token
            }
            token = strtok(0, delim);
        }
        *(result + idx) = 0;

        free(str_copy); // Free the temporary copy of the string
    }

    *count = count_temp - 1;

    return result;
}

// Helper function to free the memory allocated for a Matrix of string
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

int is_duplicate(char **array, int count, const char *value)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(array[i], value) == 0)
        {
            return 1; // duplicate value found
        }
    }
    return 0; // no value found
}

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
        if (is_duplicate(headers, i, headers[i]))
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
                        handle_error();
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
    // Printing the parameters
    if (DEBUG_LOG == 1)
    {
        printf("CSV File Path: %s\n", csvFilePath);
        printf("Selected Columns: %s\n", selectedColumns);
        printf("Row Filter Definitions: %s\n", rowFilterDefinitions);
        printf("Open file CSV\n");
        fflush(stdout); // Ensures that output is immediately written to stdout
    }

    FILE *file = fopen(csvFilePath, "r");
    if (!file)
    {
        perror("Failed to open file");
        return;
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

    fseek(file, 0, SEEK_END);  // move the position of pointer to the end
    long length = ftell(file); // gets the current position of the pointer, which corresponds to the file size
    fseek(file, 0, SEEK_SET);  // move the file pointer back to the beginning

    char *buffer = malloc(length + 1); // allocate memory + \o
    fread(buffer, 1, length, file);    // reads the contents of the file into the buffer
    buffer[length] = '\0';             // add \o the the end of file for a valid string

    fclose(file); // close file after reading

    processCsv(buffer, selectedColumns, rowFilterDefinitions);

    free(buffer); // free the memory allocated to the buffer
    if (DEBUG_LOG == 1)
    {
        printf("Finalizing CSV file\n");
        fflush(stdout);
    }
}

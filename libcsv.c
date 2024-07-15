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
#include "src/helpers/process-csv-lines.h"
#include "src/helpers/process-csv-headers.h"
#include "src/helpers/process-csv-selected.h"
#include "src/helpers/process-csv-filters.h"

int DEBUG_LOG = 0;

// Process CSV data given as a string
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    CsvLines csvLines = processCsvLines(csv);
    CsvHeader csvHeader = processCsvHeaders(csvLines.lines, csvLines.line_count);
    CsvSelection csvSelection = processCsvSelected(selectedColumns, csvHeader);
    CsvFilter csvFilter = processCsvFilters(rowFilterDefinitions, csvHeader);

    // Process each row

    // headers
    for (int k = 0; k < csvSelection.selected_count; k++)
    {
        int column_selected = csvSelection.selected_indice_column[k];
        char *header = csvHeader.headers[column_selected];

        fputs(header, stdout);
        if (k < csvSelection.selected_count - 1)
        {
            fputs(",", stdout);
        }
    }
    fputs("\n", stdout);

    // values
    for (int i = 0; i < csvLines.line_count; i++) // don't consider the headers
    {
        int row_count = 0;
        char **row = split(csvLines.lines[i], ',', &row_count);
        int match = 1;

        if (DEBUG_LOG == 1)
            printf("\ni: %d", i);

        for (int j = 0; j < csvFilter.filter_count; j++)
        {
            int column_filter = csvFilter.filter_indice_column[j];
            char *value = row[column_filter];

            if (strcmp(csvFilter.filter_operators[j], "=") == 0)
            {
                if (strcmp(value, csvFilter.filter_values[j]) == 0)
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
            else if (strcmp(csvFilter.filter_operators[j], ">") == 0)
            {
                // printf("\n____________________");
                // printf("\nvalue: %s", value);
                // printf("\nfilter_values: %s", filter_values[j]);
                // printf("\n____________________\n");
                if (strcmp(value, csvFilter.filter_values[j]) > 0)
                {
                    if (DEBUG_LOG == 1)
                        printf("\n[>] j: %d filter_indice_column: %d value: %s filter_values[j]: %s", j, column_filter, value, csvFilter.filter_values[j]);
                }
                else
                {
                    match = 0;
                    break;
                }
            }
            else if (strcmp(csvFilter.filter_operators[j], ">=") == 0)
            {
                if (strcmp(value, csvFilter.filter_values[j]) >= 0)
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
            else if (strcmp(csvFilter.filter_operators[j], "<") == 0)
            {
                if (strcmp(value, csvFilter.filter_values[j]) < 0)
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
            else if (strcmp(csvFilter.filter_operators[j], "<=") == 0)
            {
                if (strcmp(value, csvFilter.filter_values[j]) <= 0)
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
            char **result_value = malloc(csvSelection.selected_count * sizeof(char *));
            int result_count = 0;

            if (DEBUG_LOG == 1)
                printf("\n\n");

            for (int k = 0; k < csvSelection.selected_count; k++)
            {
                int column_selected = csvSelection.selected_indice_column[k];
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

    freeStringArray(csvHeader.headers);
    freeStringArray(csvFilter.filter_operators);
    // freeStringArray(lines);
    // freeStringArray(filter_values); // error in big matrix ? free(): invalid pointer \n Aborted (core dumped)
    free(csvSelection.selected_indice_column);
    free(csvFilter.filter_indice_column);
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

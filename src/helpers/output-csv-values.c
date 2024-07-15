#include <stdio.h> // para fputs e stdout
#include <stdlib.h>
#include <string.h>
#include "handle-error.h"
#include "split.h"
#include "free-string-array.h"
#include "output-csv-values.h"
extern int DEBUG_LOG;

void outputCsvValues(CsvLines csvLines, CsvFilter csvFilter, CsvSelection csvSelection)
{
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
}

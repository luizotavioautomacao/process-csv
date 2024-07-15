#include <stdlib.h> // malloc, realloc
#include <string.h> // strcmp
#include <stdio.h> 
#include "process-csv-selected.h"
#include "handle-error.h"
#include "free-string-array.h"
#include "split.h"

extern int DEBUG_LOG;

CsvSelection processCsvSelected(const char *selectedColumns, CsvHeader csvHeader)
{
    CsvSelection obj;

    obj.selected_count = 0;
    obj.selecteds = NULL;
    obj.selected_indice_column = NULL;
    obj.selecteds = split(selectedColumns, ',', &(obj.selected_count));
    obj.selected_indice_column = malloc(sizeof(int) * obj.selected_count);

    if (selectedColumns == NULL || selectedColumns[0] == '\0' || obj.selected_count == 0)
    {
        // If no columns are selected, select all
        obj.selected_count = csvHeader.column_count;
        obj.selected_indice_column = realloc(obj.selected_indice_column, sizeof(int) * csvHeader.column_count);
        for (int i = 0; i < csvHeader.column_count; i++)
        {
            obj.selected_indice_column[i] = i;
        }
    }
    else
    {
        // Determine which columns to select
        if (obj.selected_indice_column == NULL)
        {
            freeStringArray(obj.selecteds);
            obj.selecteds = NULL;
            obj.selected_indice_column = 0;
            handleError("Falha ao alocar memória para selecionar colunas\n");
        }

        for (int i = 0; i < obj.selected_count; i++)
        {
            for (int j = 0; j < csvHeader.column_count; j++)
            {
                if (strcmp(obj.selecteds[i], csvHeader.headers[j]) == 0)
                {
                    obj.selected_indice_column[i] = j;
                    break;
                }
            }
        }

        freeStringArray(obj.selecteds);
        obj.selecteds = NULL;
    }

    if (DEBUG_LOG == 1)
    {
        printf("\nselected_columns: %s \n", selectedColumns);
        printf("\nselected_count: %d \n", obj.selected_count);
        for (int i = 0; i < obj.selected_count; i++)
        {
            printf("selected_indice_column[%d]: %d\n", i, obj.selected_indice_column[i]);
            printf("selecteds: %s\n", obj.selecteds[i]);
        }
    }

    return obj;
}

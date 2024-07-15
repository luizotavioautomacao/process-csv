
#include "process-csv-filters.h"
#include <stdlib.h>            // para malloc, realloc
#include <string.h>            // para strcmp, strtok
#include "handle-error.h"      // se necessário para tratamento de erros
#include "free-string-array.h" // se necessário para liberar memória de strings

extern int DEBUG_LOG; // se necessário para debug

CsvFilter processCsvFilters(const char *rowFilterDefinitions, CsvHeader csvHeader)
{
    CsvFilter obj;
    obj.filter_count = 0;
    obj.filter_indice_column = NULL;
    obj.filter_operators = NULL;
    obj.filter_values = NULL;

    if (rowFilterDefinitions == NULL || rowFilterDefinitions[0] == '\0')
    {
        handleError("Não existe filtros!\n");
    }

    // Processar definições de filtros
    char **filtersArray = split(rowFilterDefinitions, '\n', &(obj.filter_count));

    obj.filter_indice_column = malloc(sizeof(int) * obj.filter_count);
    obj.filter_operators = malloc(sizeof(char *) * obj.filter_count);
    obj.filter_values = malloc(sizeof(char *) * obj.filter_count);

    if (obj.filter_indice_column == NULL || obj.filter_operators == NULL || obj.filter_values == NULL)
    {
        freeStringArray(filtersArray);
        handleError("Falha ao alocar memória para filtros\n");
    }

    for (int i = 0; i < obj.filter_count; i++)
    {
        char *filter = strdup(filtersArray[i]);

        // Encontrar operador
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

        // Inicializar como NULL para lidar com falhas potenciais de alocação
        obj.filter_operators[i] = NULL;
        obj.filter_values[i] = NULL;

        for (int j = 0; j < csvHeader.column_count; j++)
        {
            if (strcmp(header, csvHeader.headers[j]) == 0)
            {
                obj.filter_indice_column[i] = j;
                obj.filter_operators[i] = strdup(operator);
                obj.filter_values[i] = strdup(value);

                if (obj.filter_operators[i] == NULL || obj.filter_values[i] == NULL)
                {
                    free(obj.filter_operators[i]);
                    free(obj.filter_values[i]);
                    obj.filter_operators[i] = NULL;
                    obj.filter_values[i] = NULL;
                    free(filter);
                    freeStringArray(filtersArray);
                    handleError("Falha ao alocar memória para filtros\n");
                }

                break;
            }
        }
        free(filter);
    }

    freeStringArray(filtersArray);

    if (DEBUG_LOG == 1)
    {
        printf("\n");
        for (int i = 0; i < obj.filter_count; i++)
        {
            printf("Filter %d:\n", i + 1);
            printf("Column index: %d\n", obj.filter_indice_column[i]);
            printf("Operator: %s\n", obj.filter_operators[i]);
            printf("Value: %s\n", obj.filter_values[i]);
            printf("\n\n");
        }
    }

    return obj;
}

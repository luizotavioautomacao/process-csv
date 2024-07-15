
#ifndef PROCESS_CSV_FILTERS_H
#define PROCESS_CSV_FILTERS_H

#include "process-csv-headers.h"
#include "split.h"
#include <stdio.h> // apenas se necessário para debug

typedef struct {
    int filter_count;
    int *filter_indice_column;
    char **filter_operators;
    char **filter_values;
} CsvFilter;

CsvFilter processCsvFilters(const char *rowFilterDefinitions, CsvHeader csvHeader);

#endif /* PROCESS_CSV_FILTERS_H */

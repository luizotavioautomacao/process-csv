#ifndef PROCESS_CSV_SELECTED_H
#define PROCESS_CSV_SELECTED_H

#include "process-csv-headers.h"

typedef struct {
    int selected_count;
    char **selecteds;
    int *selected_indice_column;
} CsvSelection;

CsvSelection processCsvSelected(const char *selectedColumns, CsvHeader csvHeader);

#endif /* PROCESS_CSV_SELECTED_H */

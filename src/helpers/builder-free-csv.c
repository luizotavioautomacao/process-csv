#include <stdlib.h>
#include "builder-free-csv.h"
#include "free-string-array.h"

void freeCsvObjects(CsvLines csvLines, CsvHeader csvHeader, CsvFilter csvFilter, CsvSelection csvSelection)
{
    freeStringArray(csvHeader.headers);
    freeStringArray(csvFilter.filter_operators);
    free(csvSelection.selected_indice_column);
    free(csvFilter.filter_indice_column);
    // freeStringArray(lines);
    // freeStringArray(filter_values); // error in big matrix ? free(): invalid pointer \n Aborted (core dumped)
}

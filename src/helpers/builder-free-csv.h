#ifndef BUILDER_FREE_STRING_ARRAY_H
#define BUILDER_FREE_STRING_ARRAY_H

#include "process-csv-headers.h"
#include "process-csv-selected.h"
#include "process-csv-filters.h"
#include "process-csv-lines.h"

void freeCsvObjects(CsvLines csvLines, CsvHeader csvHeader, CsvFilter csvFilter, CsvSelection csvSelection);

#endif /* BUILDER_FREE_STRING_ARRAY_H */

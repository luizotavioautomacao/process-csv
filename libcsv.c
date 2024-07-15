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
#include "src/helpers/output-csv-headers.h"
#include "src/helpers/output-csv-values.h"
#include "src/helpers/builder-free-csv.h"

int DEBUG_LOG = 0;

// Process CSV data given as a string
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])
{
    CsvLines csvLines = processCsvLines(csv);
    CsvHeader csvHeader = processCsvHeaders(csvLines.lines, csvLines.line_count);
    CsvSelection csvSelection = processCsvSelected(selectedColumns, csvHeader);
    CsvFilter csvFilter = processCsvFilters(rowFilterDefinitions, csvHeader);
    outputCsvHeaders(csvSelection, csvHeader);
    outputCsvValues(csvLines, csvFilter, csvSelection);
    freeCsvObjects(csvLines, csvHeader, csvFilter, csvSelection);
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

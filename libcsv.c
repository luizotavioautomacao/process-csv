#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcsv.h"

// Process CSV data given as a string
void processCsv(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[]) {
    // Imprimindo os parâmetros
    printf("CSV: %s\n", csv);
    printf("Selected Columns: %s\n", selectedColumns);
    printf("Row Filter Definitions: %s\n", rowFilterDefinitions);
    fflush(stdout);  // Garante que a saída é imediatamente escrita no stdout
}

// Process CSV data from a file
void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[]) {
    // Imprimindo os parâmetros
    printf("CSV File Path: %s\n", csvFilePath);
    printf("Selected Columns: %s\n", selectedColumns);
    printf("Row Filter Definitions: %s\n", rowFilterDefinitions);
    fflush(stdout);  // Garante que a saída seja imediatamente escrita no stdout
}

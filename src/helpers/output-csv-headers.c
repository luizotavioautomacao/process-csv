#include <stdio.h> // fputs e stdout
#include "output-csv-headers.h"

void outputCsvHeaders(CsvSelection csvSelection, CsvHeader csvHeader)
{
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
}

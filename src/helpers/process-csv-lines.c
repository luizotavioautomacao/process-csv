#include <stdio.h> // printf e fprintf
#include "split.h"
#include "process-csv-lines.h"
#include "free-string-array.h"
#include "handle-error.h"

extern int DEBUG_LOG;

CsvLines processCsvLines(const char *csv)
{
    CsvLines obj;

    int line_count = 0;
    obj.lines = split(csv, '\n', &obj.line_count);

    if (obj.line_count <= 0)
    {
        freeStringArray(obj.lines);
        handleError("Algo aconteceu de errado e não foi possível processar as linhas do CSV.\n");
    }

    if (DEBUG_LOG == 1)
    {
        for (int i = 0; i < obj.line_count; i++)
        {
            printf("line%d: %s\n", i + 1, obj.lines[i]);
        }
    }

    return obj;
}

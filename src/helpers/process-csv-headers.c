#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "process-csv-headers.h"
#include "handle-error.h"
#include "split.h"
#include "free-string-array.h"
#include "is-array-duplicate.h"

extern int DEBUG_LOG;

CsvHeader processCsvHeaders(char **lines, int line_count)
{
    CsvHeader obj;
    obj.headers = NULL;
    obj.column_count = 0;

    if (line_count <= 0 || lines == NULL)
    {
        handleError("Não foi possivel processar os headers pois o CSV contém dados inválidos!\n");
    }

    obj.headers = split(lines[0], ',', &(obj.column_count));

    if (obj.column_count > 256)
    {
        freeStringArray(obj.headers);
        obj.headers = NULL;
        obj.column_count = 0;
        handleError("Esse CSV contém mais de 256 colunas!\n");
    }

    for (int i = 0; i < obj.column_count; i++)
    {
        if (isArrayDuplicate(obj.headers, i, obj.headers[i]))
        {
            freeStringArray(obj.headers);
            obj.headers = NULL;
            obj.column_count = 0;
            handleError("Esse CSV contém colunas duplicadas! Nome do header duplicado: %s\n", obj.headers[i]);
        }
    }

    if (DEBUG_LOG == 1)
    {
        for (int i = 0; i < obj.column_count; i++)
        {
            printf("headers%d: %s\n", i + 1, obj.headers[i]);
        }
    }

    return obj;
}

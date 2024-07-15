#ifndef PROCESS_CSV_HEADERS_H
#define PROCESS_CSV_HEADERS_H

typedef struct {
    char **headers;
    int column_count;
} CsvHeader;

CsvHeader processCsvHeaders(char **lines, int line_count);

#endif /* PROCESS_CSV_HEADERS_H */

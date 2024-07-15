#ifndef PROCESS_CSV_LINES_H
#define PROCESS_CSV_LINES_H

typedef struct {
    char **lines;
    int line_count;
} CsvLines;

CsvLines processCsvLines(const char *csv);

#endif /* PROCESS_CSV_LINES_H */

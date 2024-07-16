#include <dlfcn.h> // Library to load shared libraries dynamically
#include <stdio.h>
#include "../libcsv.h"

void processCsvFile(const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[]);
void processCsv(const char[], const char[], const char[]);

int main()
{
    const char csv[] = "col1,col2,col3,col4,col5,col6,col7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7\n";     // const char csv[] = "col\"1,c\"ol2,col\"3,co\"l4,col5,col\"6,col7,coluna8,coluna9\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7,valor8,valor9\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7,value8,value9\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7,v8,v9\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7,vl8,vl9\n";
    const char csvFilePath[] = "../../data.csv";
    const char selectedColumns[] = "col1,col3,col4,col7";
    const char rowFilterDefinitions[] = "col1>l1c1\ncol3>l1c3";

    printf("\nProcess CSV from a string\n");
    processCsv(csv, selectedColumns, rowFilterDefinitions);

    printf("\nProcess CSV from a file\n");
    processCsvFile(csvFilePath, selectedColumns, rowFilterDefinitions);

    return 0;
}
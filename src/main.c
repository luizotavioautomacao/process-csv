#include <stdio.h>
#include "../libcsv.h"
#include <dlfcn.h> // Biblioteca para carregar bibliotecas compartilhadas dinamicamente

int main()
{
    void *lib_handle;
    void (*processCsv)(const char[], const char[], const char[]);
    char *error;

    printf("open .so\n\n");
    // Carrega a biblioteca compartilhada
    lib_handle = dlopen("./libcsv.so", RTLD_LAZY);
    if (!lib_handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // Carrega o símbolo da função processCsv
    processCsv = dlsym(lib_handle, "processCsv");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        dlclose(lib_handle);
        return 1;
    }

    // Usa a função processCsv
    const char csv[] = "col1,col2,col3,col4\n1,2,3,4\n5,6,7,8\n9,10,11,12\n";
    const char selectedColumns[] = "col1,col3";
    const char rowFilterDefinitions[] = "col1>2";

    (*processCsv)(csv, selectedColumns, rowFilterDefinitions);

    printf("\nclose .so\n");
    // Fecha a biblioteca compartilhada
    dlclose(lib_handle);

    return 0;
}
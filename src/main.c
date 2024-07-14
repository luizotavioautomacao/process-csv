#include <dlfcn.h> // Library to load shared libraries dynamically
#include <stdio.h>
#include "../libcsv.h"

int main()
{
    void *lib_handle;
    void (*processCsv)(const char[], const char[], const char[]);
    char *error;

    printf("open .so\n\n");
    // Load the shared library
    lib_handle = dlopen("./libcsv.so", RTLD_LAZY);
    if (!lib_handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // Loads the processCsv function symbol
    processCsv = dlsym(lib_handle, "processCsv");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        dlclose(lib_handle);
        return 1;
    }

    // Use the processCsv function
    const char csv[] = "co\"l1,col2,c\"ol3,col\"4,col5,col6,col7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl1c1,l1c2,l1c3,l1c4,l1c5,l1c6,l1c7\nl2c1,l2c2,l2c3,l2c4,l2c5,l2c6,l2c7\nl3c1,l3c2,l3c3,l3c4,l3c5,l3c6,l3c7\n";
    const char selectedColumns[] = "col1,col3,col4,col7";
    const char rowFilterDefinitions[] = "col1>l1c1\ncol3>l1c3";

    (*processCsv)(csv, selectedColumns, rowFilterDefinitions);

    printf("\n\nclose .so\n");
    // Closes the shared library
    dlclose(lib_handle);

    return 0;
}
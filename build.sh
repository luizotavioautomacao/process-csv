#!/bin/sh 
# Contain the commands necessary to compile the library (.so) and the test_libcsv executable

# Compile remove-quotes.c to a shared object
gcc -fPIC -c src/helpers/remove-quotes.c -o remove-quotes.o

# Compile split.c to a shared object
gcc -fPIC -c src/helpers/split.c -o split.o

# Compile is-array-duplicate.c to a shared object
gcc -fPIC -c src/helpers/is-array-duplicate.c -o is-array-duplicate.o

# Compile free-string-array.c to a shared object
gcc -fPIC -c src/helpers/free-string-array.c -o free-string-array.o

# Compile handle-error.c to a shared object
gcc -fPIC -c src/helpers/handle-error.c -o handle-error.o

# Compile read-file-contents.c to a shared object
gcc -fPIC -c src/helpers/read-file-contents.c -o read-file-contents.o

# Compile process-csv-lines.c to a shared object
gcc -fPIC -c src/helpers/process-csv-lines.c -o process-csv-lines.o

# Compile process-csv-headers.c to a shared object
gcc -fPIC -c src/helpers/process-csv-headers.c -o process-csv-headers.o

# Compile process-csv-selected.c to a shared object
gcc -fPIC -c src/helpers/process-csv-selected.c -o process-csv-selected.o

# Compile process-csv-filters.c to a shared object
gcc -fPIC -c src/helpers/process-csv-filters.c -o process-csv-filters.o

# Compile output-csv-headers.c to a shared object
gcc -fPIC -c src/helpers/output-csv-headers.c -o output-csv-headers.o

# Compile output-csv-values.c to a shared object
gcc -fPIC -c src/helpers/output-csv-values.c -o output-csv-values.o

# Compile libcsv.c to a shared object
gcc -fPIC -c libcsv.c -o libcsv.o

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-headers.o output-csv-headers.o output-csv-values.o

# gcc -o test_libcsv test_libcsv.c -L. -lcsv # (test_libcsv is already compiled)

# Clean up object files
rm libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-headers.o output-csv-headers.o output-csv-values.o
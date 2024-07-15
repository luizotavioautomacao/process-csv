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

# Compile libcsv.c to a shared object
gcc -fPIC -c libcsv.c -o libcsv.o

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o

# gcc -o test_libcsv test_libcsv.c -L. -lcsv # (test_libcsv is already compiled)

# Clean up object files
rm libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o
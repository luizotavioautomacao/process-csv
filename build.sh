#!/bin/sh 
# Contain the commands necessary to compile the library (.so) and the test_libcsv executable

# Compile remove-quotes.c to a shared object
gcc -fPIC -c src/helpers/remove-quotes.c -o remove-quotes.o

# Compile libcsv.c to a shared object
gcc -fPIC -c libcsv.c -o libcsv.o

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o

# gcc -o test_libcsv test_libcsv.c -L. -lcsv # (test_libcsv is already compiled)
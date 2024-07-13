#!/bin/sh 
## Conter os comandos necessários para compilar a biblioteca (.so) e o executável test_libcsv ##
gcc -fPIC -c libcsv.c -o libcsv.o
gcc -shared -o libcsv.so libcsv.o
# gcc -o test_libcsv test_libcsv.c -L. -lcsv # (test_libcsv já está compilado)
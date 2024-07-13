#!/bin/sh
clear
gcc -fPIC -c ../libcsv.c -o libcsv.o
gcc -shared -o libcsv.so libcsv.o
gcc -o main main.c
./main
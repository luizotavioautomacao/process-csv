#!/bin/sh
clear

# Compile libcsv.c to an object file
gcc -fPIC -c ../libcsv.c -o libcsv.o

# Compile main.c to an object file
gcc -fPIC -c main.c -o main.o

# Compile remove-quotes.c to an object file
gcc -fPIC -c helpers/remove-quotes.c -o remove-quotes.o

# Compile split.c to a shared object
gcc -fPIC -c helpers/split.c -o split.o

# Compile is-array-duplicate.c to a shared object
gcc -fPIC -c helpers/is-array-duplicate.c -o is-array-duplicate.o

# Compile free-string-array.c to a shared object
gcc -fPIC -c helpers/free-string-array.c -o free-string-array.o

# Compile handle-error.c to a shared object
gcc -fPIC -c helpers/handle-error.c -o handle-error.o

# Compile read-file-contents.c to a shared object
gcc -fPIC -c helpers/read-file-contents.c -o read-file-contents.o

# Compile process-csv-lines.c to a shared object
gcc -fPIC -c helpers/process-csv-lines.c -o process-csv-lines.o

# Compile process-csv-headers.c to a shared object
gcc -fPIC -c helpers/process-csv-headers.c -o process-csv-headers.o

# Compile process-csv-selected.c to a shared object
gcc -fPIC -c helpers/process-csv-selected.c -o process-csv-selected.o

# Compile process-csv-filters.c to a shared object
gcc -fPIC -c helpers/process-csv-filters.c -o process-csv-filters.o

# Compile output-csv-headers.c to a shared object
gcc -fPIC -c helpers/output-csv-headers.c -o output-csv-headers.o

# Compile output-csv-values.c to a shared object
gcc -fPIC -c helpers/output-csv-values.c -o output-csv-values.o

# Compile builder-free-csv.c to a shared object
gcc -fPIC -c helpers/builder-free-csv.c -o builder-free-csv.o

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

# Link the main object file with the shared library to create the executable
gcc -o main main.o -L. -lcsv -Wl,-rpath,.

# Create the /bin directory if it doesn't exist
mkdir -p bin

# Move compiled files to the /bin directory
mv libcsv.so bin/
mv main bin/

# Clean up object files
rm main.o libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

# Change directory to /bin
cd bin || exit

# Run the program
./main

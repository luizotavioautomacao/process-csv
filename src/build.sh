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

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o

# Link the main object file with the shared library to create the executable
gcc -o main main.o -L. -lcsv -Wl,-rpath,.

# Create the /bin directory if it doesn't exist
mkdir -p bin

# Move compiled files to the /bin directory
mv libcsv.so bin/
mv main bin/

# Clean up object files
rm main.o libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o

# Change directory to /bin
cd bin || exit

# Run the program
./main

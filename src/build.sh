#!/bin/sh
clear

# Compile remove-quotes.c to an object file
gcc -fPIC -c helpers/remove-quotes.c -o remove-quotes.o

# Compile libcsv.c to an object file
gcc -fPIC -c ../libcsv.c -o libcsv.o

# Generate a shared library (libcsv.so) including remove-quotes.o
gcc -shared -o libcsv.so libcsv.o remove-quotes.o

# Create the /bin directory if it doesn't exist
mkdir -p bin

# Move compiled files to the /bin directory
mv libcsv.so bin/
mv main bin/

# Clean up object files
rm libcsv.o remove-quotes.o

# Change directory to /bin
cd bin || exit

# Run the program
./main

#!/bin/sh
clear

# Compile remove-quotes.c to an object file
gcc -fPIC -c helpers/remove-quotes.c -o remove-quotes.o

# Compile split.c to a shared object
gcc -fPIC -c helpers/split.c -o split.o

# Compile libcsv.c to an object file
gcc -fPIC -c ../libcsv.c -o libcsv.o

# Compile main.c to an object file
gcc -fPIC -c main.c -o main.o

# Generate a shared library (libcsv.so)
gcc -shared -o libcsv.so libcsv.o remove-quotes.o split.o

# Link the main object file with the shared library to create the executable
gcc -o main main.o -L. -lcsv -Wl,-rpath,.

# Create the /bin directory if it doesn't exist
mkdir -p bin

# Move compiled files to the /bin directory
mv libcsv.so bin/
mv main bin/

# Clean up object files
rm libcsv.o remove-quotes.o split.o main.o

# Change directory to /bin
cd bin || exit

# Run the program
./main

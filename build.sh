#!/bin/sh
# Contain the commands necessary to compile the library (.so) and the test_libcsv executable

apk add --no-cache build-base

echo "@community https://dl-cdn.alpinelinux.org/alpine/edge/community" >> /etc/apk/repositories
apk update

apk add --no-cache build-base gdb@community valgrind@community

echo "Compile remove-quotes.c to a shared object"
gcc -fPIC -g -c src/helpers/remove-quotes.c -o remove-quotes.o

echo "Compile split.c to a shared object"
gcc -fPIC -g -c src/helpers/split.c -o split.o

echo "Compile is-array-duplicate.c to a shared object"
gcc -fPIC -g -c src/helpers/is-array-duplicate.c -o is-array-duplicate.o

echo "Compile free-string-array.c to a shared object"
gcc -fPIC -g -c src/helpers/free-string-array.c -o free-string-array.o

echo "Compile handle-error.c to a shared object"
gcc -fPIC -g -c src/helpers/handle-error.c -o handle-error.o

echo "Compile read-file-contents.c to a shared object"
gcc -fPIC -g -c src/helpers/read-file-contents.c -o read-file-contents.o

echo "Compile process-csv-lines.c to a shared object"
gcc -fPIC -g -c src/helpers/process-csv-lines.c -o process-csv-lines.o

echo "Compile process-csv-headers.c to a shared object"
gcc -fPIC -g -c src/helpers/process-csv-headers.c -o process-csv-headers.o

echo "Compile process-csv-selected.c to a shared object"
gcc -fPIC -g -c src/helpers/process-csv-selected.c -o process-csv-selected.o

echo "Compile process-csv-filters.c to a shared object"
gcc -fPIC -g -c src/helpers/process-csv-filters.c -o process-csv-filters.o

echo "Compile output-csv-headers.c to a shared object"
gcc -fPIC -g -c src/helpers/output-csv-headers.c -o output-csv-headers.o

echo "Compile output-csv-values.c to a shared object"
gcc -fPIC -g -c src/helpers/output-csv-values.c -o output-csv-values.o

echo "Compile builder-free-csv.c to a shared object"
gcc -fPIC -g -c src/helpers/builder-free-csv.c -o builder-free-csv.o

echo "Compile libcsv.c to a shared object"
gcc -fPIC -g -c libcsv.c -o libcsv.o

echo "Generate a shared library (libcsv.so)"
gcc -shared -o libcsv.so -fPIC -g -m64 libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

echo "Compile main.c to main executable"
gcc -g -o main src/main.c -L. -lcsv -Wl,-rpath,.

# Clean up object files
echo "Clean up object files"
rm libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

# Check if the main file exists before copying it
if [ -f "/app/main" ]; then
    echo "Copying main..."
    cp /app/main /app/main
    chmod +x /app/main
else
    echo "Warning: File /app/main not found."
fi

# Check if the test_libcsv file exists before copying it and setting permissions
if [ -f "/app/test_libcsv" ]; then
    echo "Copying test_libcsv..."
    cp /app/test_libcsv /app/test_libcsv
    chmod +x /app/test_libcsv
else
    echo "Warning: File /app/test_libcsv not found."
fi

echo "Completing the build.sh ..."
echo ""

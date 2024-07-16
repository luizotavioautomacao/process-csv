#!/bin/sh 
# Contain the commands necessary to compile the library (.so) and the test_libcsv executable

# cd /app
apk add --no-cache build-base

echo "Compile remove-quotes.c to a shared object"
gcc -fPIC -c src/helpers/remove-quotes.c -o remove-quotes.o

echo "Compile split.c to a shared object"
gcc -fPIC -c src/helpers/split.c -o split.o

echo "Compile is-array-duplicate.c to a shared object"
gcc -fPIC -c src/helpers/is-array-duplicate.c -o is-array-duplicate.o

echo "Compile free-string-array.c to a shared object"
gcc -fPIC -c src/helpers/free-string-array.c -o free-string-array.o

echo "Compile handle-error.c to a shared object"
gcc -fPIC -c src/helpers/handle-error.c -o handle-error.o

echo "Compile read-file-contents.c to a shared object"
gcc -fPIC -c src/helpers/read-file-contents.c -o read-file-contents.o

echo "Compile process-csv-lines.c to a shared object"
gcc -fPIC -c src/helpers/process-csv-lines.c -o process-csv-lines.o

echo "Compile process-csv-headers.c to a shared object"
gcc -fPIC -c src/helpers/process-csv-headers.c -o process-csv-headers.o

echo "Compile process-csv-selected.c to a shared object"
gcc -fPIC -c src/helpers/process-csv-selected.c -o process-csv-selected.o

echo "Compile process-csv-filters.c to a shared object"
gcc -fPIC -c src/helpers/process-csv-filters.c -o process-csv-filters.o

echo "Compile output-csv-headers.c to a shared object"
gcc -fPIC -c src/helpers/output-csv-headers.c -o output-csv-headers.o

echo "Compile output-csv-values.c to a shared object"
gcc -fPIC -c src/helpers/output-csv-values.c -o output-csv-values.o

echo "Compile builder-free-csv.c to a shared object"
gcc -fPIC -c src/helpers/builder-free-csv.c -o builder-free-csv.o

echo "Compile libcsv.c to a shared object"
gcc -fPIC -c libcsv.c -o libcsv.o

echo "Generate a shared library (libcsv.so)"
gcc -shared -o libcsv.so -fPIC -m64 libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

# gcc -o test_libcsv test_libcsv.c -L. -lcsv # (test_libcsv is already compiled)

echo "Clean up object files"
rm libcsv.o remove-quotes.o split.o is-array-duplicate.o free-string-array.o handle-error.o read-file-contents.o process-csv-lines.o process-csv-headers.o process-csv-selected.o process-csv-filters.o output-csv-headers.o output-csv-values.o builder-free-csv.o

# Verificar se o arquivo main existe antes de copiá-lo
if [ -f "/app/main" ]; then
    echo "Copying main..."
    cp /app/main /app/main
    chmod +x /app/main
else
    echo "Warning: File /app/main not found."
fi

# Verificar se o arquivo test_libcsv existe antes de copiá-lo e definir as permissões
if [ -f "/app/test_libcsv" ]; then
    echo "Copying test_libcsv..."
    cp /app/test_libcsv /app/test_libcsv
    chmod +x /app/test_libcsv
else
    echo "Warning: File /app/test_libcsv not found."
fi

echo "Completing the build.sh ..."
echo ""
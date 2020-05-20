echo "Compiling..."
gcc  ../server.c -o ../bin/server -w -lpthread

echo "Running the script"
../bin/server

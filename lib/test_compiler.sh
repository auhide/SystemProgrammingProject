echo "Compiling..."
gcc  user_authentication.c -o ../bin/sv_file_comm -w

echo "Running the script"
../bin/sv_file_comm

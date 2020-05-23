echo "Compiling..."
gcc  file_interaction.c -o ../bin/sv_file_comm -w

echo "Running the script"
../bin/sv_file_comm

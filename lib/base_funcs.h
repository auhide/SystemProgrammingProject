#include <stdio.h> 
#include <fcntl.h> 
#include <time.h>


int substring_in_string(char *substr, char *str);

void split_string_to_array(char *string, char *delimiter, char **array);

int get_read_fd(char *filename);

int get_append_fd(char *filename);

void strtolower(char *str);

void log_print(char *mode, char *string);


void strtolower(char *str)
{
    size_t str_size = strlen(str);
    char curr_char;

    for(int i = 0; i < str_size; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] += 32;
        }
    }
}

int get_append_fd(char *filename)
{
    int fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0644); 

    if (fd < 0)
    { 
        perror("Error while opening the file!"); 
        exit(1); 
    }

    return fd;
}

int get_read_fd(char *filename)
{
    int fd = open(filename, O_RDONLY); 

    if (fd < 0)
    { 
        perror("Error while opening the file!"); 
        exit(1); 
    }

    return fd;
}

void split_string_to_array(char *string, char *delimiter, char **array)
{

    char *string_split = {0};
    int i = 0;

    string_split = strtok(string, delimiter);
    while(string_split != NULL)
    {   
        array[i++] = string_split;
        string_split = strtok(NULL, delimiter);
    }
}

int substring_in_string(char *substr, char *str)
{
    if (strstr(str, substr) != NULL)
    {
        return 1;
    }

    return 0;
}
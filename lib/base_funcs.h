#include <stdio.h> 
#include <fcntl.h> 
#include <time.h>
#include <ctype.h>
#include <regex.h>


int substring_in_string(char *substr, char *str);

void split_string_to_array(char *string, char *delimiter, char **array);

int get_read_fd(char *filename);

int get_append_fd(char *filename);

void strtolower(char *str);

void log_print(char *mode, char *string);

int regex_match(char *string, char *regex);


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

int string_is_alphabetic(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isalpha(str[i]))
        {
            return 0;
        }
    }
    
    return 1;
}


int regex_match(char *string, char *regex_string)
{
    regex_t regex;
    int reti;

    reti = regcomp(&regex, regex_string, REG_EXTENDED);
    reti = regexec(&regex, string, 0, NULL, 0);

    if (!reti)
    {
        return 1;
    }

    return 0;
}
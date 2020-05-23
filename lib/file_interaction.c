#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#include "sv_file_comm.c"

#define START_SIZE 256
#define LINE_SIZE 200
#define CHUNK_MEMORY 5
#define STORAGE_DELIMITER "\n"
#define LINE_DELIMITER "#"
#define TRIPS_COLS 1024


void get_full_storage(char *data);

int get_lines_number(char *data, int data_size);

char *split_string_to_array(char *string, char *delimiter, char **array);

int substring_in_string(char *substr, char *str);

void search_user_trips(char **entries, int entries_len, char *username, char *first_dest, char *second_dest);

void display_user_trips(char **data, int data_size, char *username);

// TODO: Implement this function
char *get_user_trips(char **data, int data_size, char *username, char **user_data)

// TODO: Create the TopX Longest or Shortest trips functionality

int main()
{
    // char *trips[TRIPS_ROWS] = {"User3", "Belgium-Netherlands", "44.621", "125.222", "79", "88"};
    // add_trip(get_append_fd(), trips);

    char *data = malloc(sizeof(char) * START_SIZE);
    int data_size;

    get_full_storage(data);
    // printf("%s", data);
    data_size = strlen(data);
    
    int lines_count = get_lines_number(data, data_size);
    printf("Lines: %d\n", lines_count);

    char *lines_arr[TRIPS_COLS] = {0};
    split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

    display_user_trips(lines_arr, lines_count, "user2");
    // search_user_trips(lines_arr, lines_count, "user1", "Bulgaria", "Hawaii");

    free(data);
    return 0;
}

void get_full_storage(char *data)
{
    
    int fd = get_read_fd();
    int current_max = START_SIZE;

    int i = 0;
    while (read(fd, &data[i], 1) == 1)
    {
        if (current_max < i)
        {
            current_max = i;
            data = realloc(data, current_max + CHUNK_MEMORY);
        }
        i++;
    }
    close(fd);

}

void display_user_trips(char **data, int data_size, char *username)
{
    int found = 0;
    char *curr_line_arr[TRIPS_ROWS];
    printf("YOUR TRIPS:\n", username);

    for (int i = 0; i < data_size; i++)
    {
        split_string_to_array(data[i], LINE_DELIMITER, curr_line_arr);

        if (substring_in_string(username, curr_line_arr[0]))
        {
            print_line_arr(curr_line_arr);
            found = 1;
        } 
    }

    if (!found) 
    {
        printf("You have no trips saved!\n");
    }
}

char **get_user_trips(char **data, int data_size, char *username, char **user_data)
{

}

int get_lines_number(char *data, int data_size)
{
    int lines = 0;

    for (int i = 0; i < data_size; i++)
    {
        if (data[i] == '\n')
        {
            lines++;
        }
    }

    return lines;
}



char *split_string_to_array(char *string, char *delimiter, char **array)
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
    if (strstr(str, substr))
    {
        return 1;
    }

    return 0;
}

void search_user_trips(char **entries,
                       int entries_len,
                       char *username, 
                       char *first_dest, 
                       char *second_dest)
{
    int found = 0;
    char *curr_line_arr[TRIPS_ROWS];
    printf("Found Trips:\n");

    for (int i = 0; i < entries_len; i++)
    {
        split_string_to_array(entries[i], LINE_DELIMITER, curr_line_arr);

        if (substring_in_string(username, curr_line_arr[0]) &&
            substring_in_string(first_dest, curr_line_arr[1]) &&
            substring_in_string(second_dest, curr_line_arr[1]))
        {
            print_line_arr(curr_line_arr);
            found = 1;
        } 
    }

    if (!found) 
    {
        printf("No matching trips were found!\n");
    }
}

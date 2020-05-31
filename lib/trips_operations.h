#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#include "base_funcs.h"
#include "constants.h"


void add_trip(int file_desc, char trips_arr[TRIPS_ROWS][TRIPS_COLS]);

void print_line_arr(char line[TRIPS_ROWS][TRIPS_COLS]);

void get_full_storage(char *data);

int get_lines_number(char *data, int data_size);

void search_user_trips(char **entries, int entries_len, char *first_dest, char *second_dest);

void display_trips(char **data, int data_size);

void add_chunks_to(char **array, int chunks_to_add);

void get_user_trips(char **data, int data_size, char *username, char **user_data);


// int main()
// {
//     char *trips[TRIPS_ROWS] = {"User3", "England-Bulgaria", "44.621", "125.222", "79", "44"};
//     add_trip(get_append_fd(STORAGE), trips);

//     char *data = malloc(sizeof(char) * START_SIZE);
//     int data_size;

//     get_full_storage(data);
//     // printf("%s", data);
//     data_size = strlen(data);
    
//     int lines_count = get_lines_number(data, data_size);
//     printf("Lines: %d\n", lines_count);

//     char *lines_arr[TRIPS_COLS] = {0};
//     split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

//     // Dinamically allocate the 2d Array for the users' information
//     char **user_data = (char **)calloc(data_size, sizeof (char *));
//     add_chunks_to(lines_count, user_data);

//     get_user_trips(lines_arr, lines_count, "user2", user_data);
//     // display_trips(user_data, lines_count);
//     search_user_trips(user_data, lines_count, "belgium", "netherlands");

    // free(data);
//     return 0;
// }

void add_chunks_to(char **array, int chunks_to_add)
{
    for (int chunk_i = 0; chunk_i < chunks_to_add; chunk_i++)
    {
        array[chunk_i] = (char *)calloc(TRIPS_COLS, sizeof(char *));
    }
}


void free_2d_array(char **array, int array_len)
{
    for (int i = 0; i < array_len; i++)
    {
        free(array[i]);
    }

    free(array);
}


void get_full_storage(char *data)
{
    
    int fd = get_read_fd(STORAGE);
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


void display_trips(char **data, int data_size)
{
    int found = 0;
    char *curr_line_arr[TRIPS_ROWS];
    printf("YOUR TRIPS:\n");

    for (int i = 0; i < data_size; i++)
    {
        if (data[i] != NULL)
        {
            split_string_to_array(data[i], LINE_DELIMITER, curr_line_arr);

            print_line_arr(curr_line_arr);
            found = 1;
        }
    }

    if (!found) 
    {
        printf("You have no trips saved!\n");
    }
}


void get_user_trips(char **data, int data_size, char *username, char **user_data)
{
    int found = 0;
    int user_data_index = 0;

    for (int i = 0; i < data_size; i++)
    {
        if (substring_in_string(username, data[i]))
        {
            strcpy(user_data[user_data_index++], data[i]);        
        }
        
    }
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


void search_user_trips(char **entries,
                       int entries_len,
                       char *first_dest, 
                       char *second_dest)
{
    int found = 0;
    char *curr_line_arr[TRIPS_ROWS];
    printf("Found Trips:\n");

    for (int i = 0; i < entries_len; i++)
    {
        if (entries[i] != NULL)
        {
            split_string_to_array(entries[i], LINE_DELIMITER, curr_line_arr);

            if (substring_in_string(first_dest, curr_line_arr[1]) &&
                substring_in_string(second_dest, curr_line_arr[1]))
            {
                print_line_arr(curr_line_arr);
                found = 1;
            }      
        }
    }

    if (!found) 
    {
        printf("No matching trips were found!\n");
    }
}


void add_trip(int file_desc, char trips_arr[TRIPS_ROWS][TRIPS_COLS])
{
    char current_snippet[TRIPS_COLS] = {0};
    print_line_arr(trips_arr);

    printf("Started adding...\n");
    for (int row = 0; row < TRIPS_ROWS; row++)
    {
        strcat(current_snippet, trips_arr[row]);
        if (row != TRIPS_ROWS - 1)
        {
            strcat(current_snippet, "#");
        }

        // Doesn't lowercase the username
        if (row != 0)
        {
            strtolower(current_snippet);
        }
        write(file_desc, current_snippet, strlen(current_snippet));
        memset(current_snippet, 0, sizeof(current_snippet));
    }

    write(file_desc, "\n", 1);
    printf("Trip added successfully!\n");
}


void print_line_arr(char line[TRIPS_ROWS][TRIPS_COLS])
{
    printf("-----------------------------\n");

    for (int curr_trip_stats = 0; curr_trip_stats < TRIPS_ROWS; curr_trip_stats++)
    {
        switch(curr_trip_stats)
        {
            case USER:
                printf("User: %s\n", line[curr_trip_stats]);
                break;
            case DESTINATIONS:
                printf("Trip: %s\n", line[curr_trip_stats]);
                break;
            case LATITUDE1:
                printf("First Latitude: %s\n", line[curr_trip_stats]);
                break;
            case LONGITUDE1:
                printf("First Longitude: %s\n", line[curr_trip_stats]);
                break;
            case LATITUDE2:
                printf("Second Latitude: %s\n", line[curr_trip_stats]);
                break;
            case LONGITUDE2:
                printf("Second Longitude: %s\n", line[curr_trip_stats]);
                break;
            case AVG_SPEED:
                printf("Average Speed: %skm/h\n", line[curr_trip_stats]);
                break;
            case DISTANCE:
                printf("Distance: %skm\n", line[curr_trip_stats]);
                break;
        }
    }
    printf("-----------------------------\n");
}
#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#include "../base/constants.h"


int add_trip(int file_desc, char trips_arr[TRIPS_ROWS][TRIPS_COLS], pthread_mutex_t lock);

void print_line_arr(char line[TRIPS_ROWS][TRIPS_COLS]);

void get_full_storage(char *data);

int get_lines_number(char *data, int data_size);

void search_user_trips(char entries[TRIPS_ROWS][TRIPS_COLS],
                       int entries_len,
                       char *username,
                       char *first_dest, 
                       char *second_dest);

void display_trips(char data[TRIPS_ROWS][TRIPS_COLS], int data_size);

void add_chunks_to(char **array, int chunks_to_add);

void get_user_trips(char data[TRIPS_ROWS][TRIPS_COLS], int data_size, char *username, char user_data[TRIPS_ROWS][TRIPS_COLS]);



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


void display_trips(char data[TRIPS_ROWS][TRIPS_COLS], int data_size)
{
    int found = 0;
    char curr_line_arr[TRIPS_ROWS][TRIPS_COLS];
    printf("\n\n---------------\n");
    printf("YOUR TRIPS:\n");
    printf("---------------\n");

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


void get_user_trips(char data[TRIPS_ROWS][TRIPS_COLS], int data_size, char *username, char user_data[TRIPS_ROWS][TRIPS_COLS])
{
    int found = 0;
    int user_data_index = 0;
    int counter = 0;
    

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


void search_user_trips(char entries[TRIPS_ROWS][TRIPS_COLS],
                       int entries_len,
                       char *username,
                       char *first_dest, 
                       char *second_dest)
{
    int found = 0;
    char curr_line_arr[TRIPS_ROWS][TRIPS_COLS];


    for (int i = 0; i < entries_len; i++)
    {
        if (entries[i] != NULL)
        {
            split_string_to_array(entries[i], LINE_DELIMITER, curr_line_arr);

            if (substring_in_string(username, curr_line_arr[USER]) &&
                substring_in_string(first_dest, curr_line_arr[DESTINATIONS]) &&
                substring_in_string(second_dest, curr_line_arr[DESTINATIONS]))
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


int add_trip(int file_desc, char trips_arr[TRIPS_ROWS][TRIPS_COLS], pthread_mutex_t lock)
{
    char current_snippet[TRIPS_COLS] = {0};
    print_line_arr(trips_arr);

    printf("Started adding...\n");

    // Lock Acquiring
    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);

    printf("Trip added successfully!\n");
    return 1;
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
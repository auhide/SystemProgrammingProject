#include <stdio.h>
#include <unistd.h>

#include "trips_operations.h"


int main()
{
    
    char trips[TRIPS_ROWS][TRIPS_COLS];
    // char **trips = (char **)calloc(TRIPS_ROWS * TRIPS_COLS, sizeof (char *));
    // add_chunks_to(trips, 2);

    strcpy(trips[0], "user2");
    strcpy(trips[1], "England-Bulgaria");
    strcpy(trips[2], "44.621");
    strcpy(trips[3], "125.222");
    strcpy(trips[4], "55.621");
    strcpy(trips[5], "125.222");
    strcpy(trips[6], "79");
    strcpy(trips[7], "44");

    //, "79", "44"};

    // print_line_arr(trips);

    add_trip(get_append_fd(STORAGE), trips);

    // char *data = malloc(sizeof(char) * START_SIZE);
    // int data_size;

    // get_full_storage(data);
    // // printf("%s", data);
    // data_size = strlen(data);
    
    // int lines_count = get_lines_number(data, data_size);
    // printf("Lines: %d\n", lines_count);

    // char *lines_arr[TRIPS_COLS] = {0};
    // split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

    // // // // Dinamically allocate the 2d Array for the users' information
    // char **user_data = (char **)calloc(data_size, sizeof (char *));
    // add_chunks_to(user_data, lines_count);

    // get_user_trips(lines_arr, lines_count, "user2", user_data);
    // // display_trips(user_data, lines_count);
    // search_user_trips(user_data, lines_count, "england", "bulgaria");
    
    // free_2d_array(user_data, lines_count);
    // free(data);
    return 0;
}
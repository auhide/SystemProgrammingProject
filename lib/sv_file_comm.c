#include <stdio.h> 
#include <fcntl.h> 

#define TRIPS_ROWS 6
#define TRIPS_COLS 1024
#define STORAGE "storage.txt"

#define USER 0
#define DESTINATIONS 1
#define LATITUDE 2
#define LONGITUDE 3
#define AVG_SPEED 4
#define DISTANCE 5


void add_trip(int file_desc, char **trips_arr);

int get_rw_fd();

void print_line_arr(char **line);

void find_trip(char *username, char destinations);

void strtolower(char *str);

// int main() 
// {
//     char *trips[TRIPS_ROWS] = {"user1", "Hawaii-Bulgaria", "12.423", "43.222", "60", "200"};
    
//     int fd = get_rw_fd();
//     add_trip(fd, trips);

//     close(fd);

//     return 0;
// }


void add_trip(int file_desc, char **trips_arr)
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


int get_append_fd()
{
    int fd = open(STORAGE, O_WRONLY | O_APPEND | O_CREAT, 0644); 

    if (fd < 0)
    { 
        perror("Error while opening the file!"); 
        exit(1); 
    }

    return fd;
}

int get_read_fd()
{
    int fd = open(STORAGE, O_RDONLY); 

    if (fd < 0)
    { 
        perror("Error while opening the file!"); 
        exit(1); 
    }

    return fd;
}





void print_line_arr(char **line)
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
            case LATITUDE:
                printf("Latitude: %s\n", line[curr_trip_stats]);
                break;
            case LONGITUDE:
                printf("User: %s\n", line[curr_trip_stats]);
                break;
            case AVG_SPEED:
                printf("Average Speed: %s\n", line[curr_trip_stats]);
                break;
            case DISTANCE:
                printf("Distance: %s\n", line[curr_trip_stats]);
                break;
        }
    }
    printf("-----------------------------\n");
}


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
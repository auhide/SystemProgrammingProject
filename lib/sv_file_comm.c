#include<stdio.h> 
#include <fcntl.h> 

#define TRAVEL_ROWS 6
#define TRAVEL_COLS 1024
#define FILENAME "storage.txt"

void add_trip(int file_desc, char **trips_arr);

int get_fd();

int main() 
{
    char *trips[TRAVEL_ROWS] = {"user1", "12.423", "43.222", "Hawaii-Bulgaria", "60", "200"};
    
    int fd = get_fd();
    add_trip(fd, trips);

    close(fd); 
}


void add_trip(int file_desc, char **trips_arr)
{
    char current_snippet[TRAVEL_COLS] = {0};

    printf("Started adding...\n");
    for (int row = 0; row < TRAVEL_ROWS; row++)
    {
        strcat(current_snippet, trips_arr[row]);
        if (row != TRAVEL_ROWS - 1)
        {
            strcat(current_snippet, "#");
        }

        write(file_desc, current_snippet, strlen(current_snippet));
        memset(current_snippet, 0, sizeof(current_snippet));
    }

    write(file_desc, "\n", 1);
    printf("Trip added successfully!\n");
}


int get_fd()
{
    int fd = open(FILENAME, O_WRONLY | O_APPEND | O_CREAT, 0644); 

    if (fd < 0)
    { 
        perror("Error while opening the file!"); 
        exit(1); 
    }

    return fd;
}
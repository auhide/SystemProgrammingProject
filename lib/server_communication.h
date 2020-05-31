#include "user_authentication.c"
#include "trips_operations.h"


void communicate(int sockfd);

void input_validation(int sockfd, char *buff, int (*validation_func)(char *));

int init_input_is_valid(char *input);

void validate_registration(int sockfd, char *buff);

int username_is_valid(char *username);

int login_is_valid(char *username);

void trips_interaction(int sockfd, char *buff);

int trips_menu_choice_is_valid(char *choice);

void trip_addition(int sockfd, char *username);

int validate_destination(char *dest);

int validate_coordinate(char *coord);

int validate_distance(char *dist);

int validate_speed(char *dist);

void insert_trip_info_to_array(char trip_line[TRIPS_ROWS][TRIPS_COLS],
                               char *username,
                               char *destinations,
                               char *lat1,
                               char *lon1,
                               char *lat2,
                               char *lon2,
                               char *speed,
                               char *dist);


void communicate(int sockfd) 
{ 
    char buff[MAX] = {0};
    int n;

    input_validation(sockfd, buff, init_input_is_valid);
    printf("SERVER BUFFER: %s\n", buff);
    
    if (strcmp(REGISTRATION, buff) == 0)
    {
        input_validation(sockfd, buff, username_is_valid);
    }
    else
    {
        input_validation(sockfd, buff, login_is_valid);
        trips_interaction(sockfd, buff);
    }
    
} 

void input_validation(int sockfd, char *buff, int (*validation_func)(char *))
{
    int valid = 0;
    bzero(buff, MAX);

    do
    {
        recv_msg(sockfd, buff);

        if (validation_func(buff))
        {
            printf("Server: Response 1\n");
            write(sockfd, "1", sizeof(buff));
            valid = 1;
        } 
        else
        {
            printf("Server: Response 0\n");
            write(sockfd, "0", sizeof(buff));
            bzero(buff, MAX);
            strcpy(buff, "0");
        }

    } while (!valid);
}


void trips_interaction(int sockfd, char *buff)
{
    char username[MAX];
    strcpy(username, buff);
    bzero(buff, MAX);

    input_validation(sockfd, buff, trips_menu_choice_is_valid);
    
    // Using if-else, because I am working with strings
    if (strcmp("1", buff) == 0)
    {
        trip_addition(sockfd, username);
        // Do some adding stuff here
    }

}


void trip_addition(int sockfd, char *username)
{
    char destinations[MAX];
    char dest1[MAX];
    char dest2[MAX];
    char lat1[MAX];
    char lon1[MAX];
    char lat2[MAX];
    char lon2[MAX];
    char speed[MAX];
    char dist[MAX];

    // Getting the First Destination
    input_validation(sockfd, dest1, validate_destination);
    // Getting the First Latitude and Longitude
    input_validation(sockfd, lat1, validate_coordinate);
    input_validation(sockfd, lon1, validate_coordinate);

    // Getting the Second Destination
    input_validation(sockfd, dest2, validate_destination);
    // Getting the Second Latitude and Longitude
    input_validation(sockfd, lat2, validate_coordinate);
    input_validation(sockfd, lon2, validate_coordinate);

    // Getting the Speed
    input_validation(sockfd, speed, validate_speed);

    // Getting the Distance
    input_validation(sockfd, dist, validate_distance);
    
    strcat(destinations, dest1);
    strcat(destinations, "-");
    strcat(destinations, dest2);
    printf("Destinations: %s", destinations);

    char trips[TRIPS_ROWS][TRIPS_COLS];
    insert_trip_info_to_array(trips,
                              username,
                              destinations,
                              lat1,
                              lon1,
                              lat2,
                              lon2,
                              speed,
                              dist);

    add_trip(get_append_fd(STORAGE), trips);
}

void insert_trip_info_to_array(char trip_line[TRIPS_ROWS][TRIPS_COLS],
                               char *username,
                               char *destinations,
                               char *lat1,
                               char *lon1,
                               char *lat2,
                               char *lon2,
                               char *speed,
                               char *dist)
{

    strcpy(trip_line[USER], username);
    strcpy(trip_line[DESTINATIONS], destinations);
    strcpy(trip_line[LATITUDE1], lat1);
    strcpy(trip_line[LONGITUDE1], lon1);
    strcpy(trip_line[LATITUDE2], lat2);
    strcpy(trip_line[LONGITUDE2], lon2);
    strcpy(trip_line[AVG_SPEED], speed);
    strcpy(trip_line[DISTANCE], dist);
}

int validate_distance(char *dist)
{
    if (regex_match(dist, DIST_REGEX))
    {
        return 1;
    }

    return 0;
}


int validate_coordinate(char *coord)
{
    if (regex_match(coord, COORD_REGEX))
    {
        return 1;
    }

    return 0;
}


int validate_speed(char *speed)
{
    if (regex_match(speed, SPEED_REGEX))
    {
        return 1;
    }

    return 0;
}


int validate_destination(char *dest)
{
    if (string_is_alphabetic(dest))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int init_input_is_valid(char *input)
{

    if (!substring_in_string("~", input) && strncmp("\n", input, 1) != 0)
    {
        if (substring_in_string(input, INIT_VALID_INPUTS))
        {
            return 1;
        }
    }

    return 0;
}


int username_is_valid(char *username)
{
    int fd = get_append_fd(USERS_FILE);
    int user_added_successfully;

    user_added_successfully = add_user(fd, username);
    close(fd);

    if (user_added_successfully && 
        strlen(username) > 3 && 
        strlen(username) < 9)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}


int login_is_valid(char *username)
{
    int fd = get_read_fd(USERS_FILE);
    int user_is_registered = validate_user(fd, username);

    if (user_is_registered)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int trips_menu_choice_is_valid(char *choice)
{
    if (substring_in_string(choice, TRIPS_MENU_CHOICES) && 
        !substring_in_string("~", choice))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
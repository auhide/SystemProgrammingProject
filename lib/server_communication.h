#include "base/base_funcs.h"
#include "user_authentication.h"
#include "trips/trips_operations.h"
#include "validation_funcs.h"


void communicate(int sockfd);

void validate_registration(int sockfd, char *buff);

void trips_interaction(int sockfd, char *buff);

void trip_addition(int sockfd, char *username);

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


void trips_interaction(int sockfd, char *buff)
{
    char username[MAX];
    strcpy(username, buff);
    bzero(buff, MAX);

    while(1)
    {
        input_validation(sockfd, buff, trips_menu_choice_is_valid);
        
        // Using if-else, because I am working with strings
        if (strcmp("1", buff) == 0)
        {
            trip_addition(sockfd, username);
        }
        else if (strcmp("2", buff) == 0)
        {
            // This section is managed by the client side
        }
        else if (strcmp("3", buff) == 0)
        {

        }
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
    
    memset(destinations, 0, sizeof(destinations));
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

    if(add_trip(get_append_fd(STORAGE), trips))
    {
        send_cust_msg(sockfd, "1");
    }
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
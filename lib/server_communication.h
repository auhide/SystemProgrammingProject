#include "base/base_funcs.h"
#include "user_authentication.h"
#include "trips/trips_operations.h"
#include "validation_funcs.h"


void communicate(int sockfd, pthread_mutex_t lock);

void trips_interaction(int sockfd, char *buff, pthread_mutex_t lock);

void trip_addition(int sockfd, char *username, pthread_mutex_t lock);

void insert_trip_info_to_array(char trip_line[TRIPS_ROWS][TRIPS_COLS],
                               char *username,
                               char *destinations,
                               char *lat1,
                               char *lon1,
                               char *lat2,
                               char *lon2,
                               char *speed,
                               char *dist);


void communicate(int sockfd, pthread_mutex_t lock) 
{ 
    char buff[MAX] = {0};
    int n;

    input_validation(sockfd, buff, lock, init_input_is_valid);
    printf("SERVER BUFFER: %s\n", buff);
    
    if (strcmp(REGISTRATION, buff) == 0)
    {
        input_validation(sockfd, buff, lock, username_is_valid);
    }
    else
    {
        input_validation(sockfd, buff, lock, login_is_valid);
        trips_interaction(sockfd, buff, lock);
    }
    
} 


void trips_interaction(int sockfd, char *buff, pthread_mutex_t lock)
{
    char username[MAX];
    strcpy(username, buff);
    bzero(buff, MAX);

    while(1)
    {
        input_validation(sockfd, buff, lock, trips_menu_choice_is_valid);
        
        // Using if-else, because I am working with strings
        if (strcmp("1", buff) == 0)
        {
            trip_addition(sockfd, username, lock);
        }
        else if (strcmp("2", buff) == 0)
        {
            // This section is managed by the Client side
        }
        else if (strcmp("3", buff) == 0)
        {
            // This section is managed by the Client Side
        }
        else if (strcmp("4", buff) == 0)
        {
            // This section is managed by the Client Side
        }
    }

}


void trip_addition(int sockfd, char *username, pthread_mutex_t lock)
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
    input_validation(sockfd, dest1, lock, validate_destination);
    // Getting the First Latitude and Longitude
    input_validation(sockfd, lat1, lock, validate_coordinate);
    input_validation(sockfd, lon1, lock, validate_coordinate);

    // Getting the Second Destination
    input_validation(sockfd, dest2, lock, validate_destination);
    // Getting the Second Latitude and Longitude
    input_validation(sockfd, lat2, lock, validate_coordinate);
    input_validation(sockfd, lon2, lock, validate_coordinate);

    // Getting the Speed
    input_validation(sockfd, speed, lock, validate_speed);

    // Getting the Distance
    input_validation(sockfd, dist, lock, validate_distance);
    
    memset(destinations, 0, sizeof(destinations));
    strcat(destinations, dest1);
    strcat(destinations, "-");
    strcat(destinations, dest2);

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

    if(add_trip(get_append_fd(STORAGE), trips, lock))
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
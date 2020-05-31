
#include "user_authentication.h"
#include "base_funcs.h"
#include "trips_operations.h"


void communicate(int sockfd);

int init_input_is_valid(char *input);

void display_init_options();

void init_input_validation(int sockfd, char *buff);

void registration_form(int sockfd, char *buff);

void login_form(int sockfd, char *buff);

void options_template(char *string);

void trips_interaction(int sockfd, char *buff);

void single_interaction(int sockfd, char *buff, char *input, char *output);

void trip_addition(int sockfd);

void display_user_trips(char *username);

void find_trip(char *username);




void communicate(int sockfd) 
{ 
    char buff[MAX] = {0};

    system("clear");
    printf("Welcome to Trippy\n");
    
    display_init_options();

    init_input_validation(sockfd, buff);
    printf("CLIENT BUFFER: %s\n", buff);

    if (strcmp(REGISTRATION, buff) == 0)
    {
        registration_form(sockfd, buff);
    }
    else
    {
        login_form(sockfd, buff);
        trips_interaction(sockfd, buff);
    }
    // for (;;)
	// {
    //     write(sockfd, buff, sizeof(buff));
        
    //     bzero(buff, sizeof(buff)); 
    //     read(sockfd, buff, sizeof(buff)); 
    //     printf("From Server : %s", buff); 
        
    //     if ((strncmp(buff, "exit", 4)) == 0) { 
    //         printf("Client Exit...\n"); 
    //         break; 
    //     } 
    // } 
}


void display_init_options()
{
    printf("1. Register\n");
    printf("2. Login\n");
}

void options_template(char *string)
{
    printf("\n\n");
    printf("-----------------\n");
    printf("%s\n", string);
    printf("-----------------\n");
}

void init_input_validation(int sockfd, char *buff)
{
    char resp_buff[MAX];

    do
    {
        // Sending the initial choice to the Server
        printf("Enter choice: "); 
        send_msg(sockfd, buff);

        // Receiving the response from the Server
        recv_msg(sockfd, resp_buff);
        printf("SERVER RESPONSE: %s\n", resp_buff);

        if (substring_in_string("0", resp_buff))
        {
            printf("Wrong input, try again!\n");
        }

    } while (substring_in_string("0", resp_buff));
}

void registration_form(int sockfd, char *buff)
{   
    system("clear");
    options_template("Registration");

    single_interaction(sockfd, buff, 
                       "Enter username: ", 
                       "Your username has to be of a length between 4 and 8, inclusively!");

}

void login_form(int sockfd, char *buff)
{
    system("clear");
    options_template("Login");
    
    single_interaction(sockfd, buff, 
                       "Enter username: ", 
                       "This username does not exist!");

}

void trips_interaction(int sockfd, char *buff)
{
    char username[MAX];
    strcpy(username, buff);

    while(1)
    {
        bzero(buff, MAX);
        printf("\n\n");
        printf("1. Add new trip\n");
        printf("2. Display all my trips\n");
        printf("3. Display a certain trip of mine\n");
        printf("4. Display TopX trips\n");

        // Getting the initial trips option choice
        single_interaction(sockfd, buff, 
                           "Enter choice: ", 
                           "This choice is not corrent!");

        if (strcmp("1", buff) == 0)
        {
            trip_addition(sockfd);
        }
        else if (strcmp("2", buff) == 0)
        {
            display_user_trips(username);
        }
        else if (strcmp("3", buff) == 0)
        {
            find_trip(username);
        }
        else if (strcmp("4", buff) == 0)
        {
            // TODO: Implement TopX here
        }
        else if (strcmp("exit", buff) == 0)
        {
            exit(1);
        }
    }
}


void find_trip(char *username)
{
    char dest1[MAX];
    char dest2[MAX];
    
    options_template("Search a Trip");

    printf("First Destination: ");
    scanf("%s", dest1);
    strtolower(dest1);

    printf("Second Destination: ");
    scanf("%s", dest2);
    strtolower(dest2);

    // TODO: Make this into a separate function
    char *data = malloc(sizeof(char) * START_SIZE);
    int data_size;

    get_full_storage(data);
    data_size = strlen(data);

    int lines_count = get_lines_number(data, data_size);
    
    char lines_arr[TRIPS_ROWS][TRIPS_COLS] = {0};
    split_string_to_array(data, STORAGE_DELIMITER, lines_arr);
    ////////////////////////////////////////////////////

    search_user_trips(lines_arr, lines_count, username, dest1, dest2);
}

void display_user_trips(char *username)
{
    // TODO: Make some of it into a separate function
    char *data = malloc(sizeof(char) * START_SIZE);
    int data_size;

    get_full_storage(data);
    data_size = strlen(data);

    int lines_count = get_lines_number(data, data_size);
    printf("You have %d trips!\n", lines_count);
    
    char lines_arr[TRIPS_ROWS][TRIPS_COLS] = {0};
    split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

    // Dinamically allocate the 2d Array for the users' information
    char user_data[TRIPS_ROWS][TRIPS_COLS];

    get_user_trips(lines_arr, lines_count, username, user_data);
    display_trips(user_data, lines_count);
}

void trip_addition(int sockfd)
{
    char curr_input[MAX];

    // Sending First Destination
    single_interaction(sockfd, curr_input,
                       "From: ",
                       "Wrong destination format!");

    // Sending Latitude
    single_interaction(sockfd, curr_input,
                       "Latitude: ",
                       "Wrong latitude format!");

    // Sending Longitude
    single_interaction(sockfd, curr_input,
                       "Longitude: ",
                       "Wrong longitude format!");
    
    // Sending Second Destination
    single_interaction(sockfd, curr_input,
                       "To: ",
                       "Wrong destination format!");

    // Sending Latitude
    single_interaction(sockfd, curr_input,
                       "Latitude: ",
                       "Wrong latitude format!");

    // Sending Longitude
    single_interaction(sockfd, curr_input,
                       "Longitude: ",
                       "Wrong longitude format!");

    // Sending Speed
    single_interaction(sockfd, curr_input,
                       "Average Speed: ",
                       "Wrong Speed format!");

    // Sending Distance
    single_interaction(sockfd, curr_input,
                       "Distance: ",
                       "Wrong Distance format!");

    recv_msg(sockfd, curr_input);
    printf("%s", curr_input);

    if (strcmp(curr_input, "1") == 0)
    {
        printf("Your trip has been added");
    }
    
}


void single_interaction(int sockfd, char *buff, char *input, char *output)
{
    char resp_buff[MAX];

    do
    {
        // Sending the initial choice to the Server
        printf("%s", input); 
        send_msg(sockfd, buff);

        // Receiving the response from the Server
        recv_msg(sockfd, resp_buff);
        printf("SERVER RESPONSE: |%s|\n", resp_buff);

        if (substring_in_string("0", resp_buff))
        {
            printf("%s\n", output);
        }

    } while (substring_in_string("0", resp_buff));
}
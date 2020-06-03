
#include "user_authentication.h"
#include "base/base_funcs.h"
#include "trips/trips_operations.h"
#include "trips/topX_functionality.h"


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

void get_topx(char *username);



void communicate(int sockfd) 
{ 
    char buff[MAX] = {0};

    while(1)
    {
        system("clear");
        printf("Welcome to Trippy\n");
        printf("Enter 'exit' if you want to leave\n");
        
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
    }
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
        if (strcmp(buff, EXIT) == 0)
        {
            exit(1);
        }

        // Receiving the response from the Server
        recv_msg(sockfd, resp_buff);

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
            get_topx(username);
        }
        else if (strcmp(buff, EXIT) == 0)
        {
            exit(1);
        }
    }
}


void get_topx(char *username)
{
    char *data = malloc(sizeof(char) * START_SIZE);
    int data_size;

    get_full_storage(data);
    data_size = strlen(data);

    int lines_count = get_lines_number(data, data_size);
    
    char lines_arr[TRIPS_ROWS][TRIPS_COLS] = {0};
    split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

    char user_data[TRIPS_ROWS][TRIPS_COLS];

    get_user_trips(lines_arr, lines_count, username, user_data);
    
    int user_trips_count = count_user_trips(user_data, lines_count);

    sort_by_distances(user_data, user_trips_count);

    char type[MAX];
    int x;
    if (user_trips_count)
    {
        do
        {
            printf("Enter TopX type [longest/shortest]: ");
            scanf("%s", type);
            printf("Enter X: ");
            scanf("%d", &x);
        } while (strcmp("shortest\n", type) == 0 || strcmp("longest\n", type) == 0);

        display_top(type, x, user_data, user_trips_count);
    }
    else
    {
        printf("You have no saved trips!");
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

    free(data);
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
    
    char lines_arr[TRIPS_ROWS][TRIPS_COLS] = {0};
    split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

    char user_data[TRIPS_ROWS][TRIPS_COLS] = {0};

    get_user_trips(lines_arr, lines_count, username, user_data);
    int user_trips_count = count_user_trips(user_data, lines_count);

    display_trips(user_data, user_trips_count);

    free(data);
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

        if (substring_in_string("0", resp_buff))
        {
            printf("%s\n", output);
        }

    } while (substring_in_string("0", resp_buff));
}
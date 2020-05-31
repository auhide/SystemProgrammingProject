
#include "user_authentication.c"
#include "base_funcs.h"

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
    bzero(buff, MAX);

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
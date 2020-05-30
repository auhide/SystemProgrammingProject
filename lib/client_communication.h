
#include "user_authentication.c"


void communicate(int sockfd);

int init_input_is_valid(char *input);

void display_init_options();

void init_input_validation(int sockfd, char *buff);

void registration_form(int sockfd, char *buff);


void communicate(int sockfd) 
{ 
    char buff[MAX] = {0};

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
        printf("LOGIN");
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


void init_input_validation(int sockfd, char *buff)
{
    printf("SOCKET: %d", sockfd);

    do
    {
        // Sending the initial choice to the Server
        printf("Enter choice: "); 
        send_msg(sockfd, buff);

        // Receiving the response from the Server
        recv_msg(sockfd, buff);
        printf("SERVER RESPONSE: %s\n", buff);

        if (substring_in_string("0", buff))
        {
            printf("Wrong input, try again!\n");
        }

    } while (substring_in_string("0", buff));
}

void registration_form(int sockfd, char *buff)
{
    printf("SOCKET: %d", sockfd);
    // bzero(buff, MAX);
    // strcpy(buff, "0");

    do
    {
        // Sending the initial choice to the Server
        printf("Enter username: "); 
        send_msg(sockfd, buff);

        // Receiving the response from the Server
        recv_msg(sockfd, buff);
        printf("SERVER RESPONSE: |%s|\n", buff);

        if (substring_in_string("0", buff))
        {
            printf("Wrong input, try again!\n");
        }

    } while (substring_in_string("0", buff));

    bzero(buff, MAX);
    strcpy(buff, "1");

}


// void login(int sockfd, char *buff)
// {

// }
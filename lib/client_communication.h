
#include "user_authentication.c"


void communicate(int sockfd);

int init_input_is_valid(char *input);

void display_init_options();

void init_input_validation(int sockfd, char *buff);

void registration_form(int sockfd, char *buff);


void communicate(int sockfd) 
{ 
    char buff[MAX];

    printf("Welcome to Trippy\n");
    
    display_init_options();

    // TODO: FIX VISUALIZATION MESSAGE HERE
    init_input_validation(sockfd, buff);
    recv_msg(sockfd, buff);
    printf("CLIENT BUFFER: %s\n", buff);

    if (substring_in_string(REGISTRATION, buff))
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
    int success = 0;

    printf("Registration Form\n");
    printf("Your username has to be greater than 3 and less than 8 symbols\n\n");

    do
    {
        printf("Enter username: ");
        send_msg(sockfd, buff);
        // bzero(buff, sizeof(buff));
        // scanf("%s", buff);
        // if (strlen(buff) )

        recv_msg(sockfd, buff);
        printf("CLIENT STATUS: %s", buff);
        if (substring_in_string("0", buff) == "0")
        {
            printf("Either your username is too short/long or it has already been registered!\n");
            success = 1;
        }

    } while (substring_in_string("0", buff) == "0");

    printf("Registration successful, you can Login now!\n");
}


void login(int sockfd, char *buff)
{

}
#include "user_authentication.c"


void communicate(int sockfd);

void init_input_validation(int sockfd, char *buff);

int init_input_is_valid(char *input);

void validate_registration(int sockfd, char *buff);


void communicate(int sockfd) 
{ 
    char buff[MAX]; 
    int n;

    // TODO: FIX VISUALIZATION MESSAGE HERE
    init_input_validation(sockfd, buff);
    write(sockfd, buff, sizeof(buff));
    printf("BUFFER CLIENT: %s\n", buff);

    if (substring_in_string("1", buff) == "1")
    {
        bzero(buff, MAX);
        validate_registration(sockfd, buff);
    }
    else
    {
        printf("LOGIN\n");
    }

    // for (;;) { 
    //     bzero(buff, MAX); 

    //     read(sockfd, buff, sizeof(buff));
    //     // print buffer which contains the client contents 
    //     printf("From client: %s\t To client : ", buff); 
    //     bzero(buff, MAX);
    //     n = 0;
    //     // copy server message in the buffer
    //     while ((buff[n++] = getchar()) != '\n'); 
  
    //     // and send that buffer to client 
    //     write(sockfd, buff, sizeof(buff)); 
  
    //     // if msg contains "Exit" then server exit and chat ended. 
    //     if (strncmp("exit", buff, 4) == 0) { 
    //         printf("Server Exit...\n"); 
    //         break; 
    //     } 
    // } 
} 

void init_input_validation(int sockfd, char *buff)
{
    int valid = 0;

    do
    {
        recv_msg(sockfd, buff);

        if (init_input_is_valid(buff))
        {
            write(sockfd, "1", sizeof(buff));
            valid = 1;
        } 
        else
        {
            write(sockfd, "0", sizeof(buff));
        }

    } while (!valid);
}

int init_input_is_valid(char *input)
{
    if (!substring_in_string("~", input) && strcmp(input, "\n") != 0)
    {
        if (substring_in_string(input, INIT_VALID_INPUTS))
        {
            return 1;
        }
    }

    return 0;
}

void validate_registration(int sockfd, char *buff)
{
    int fd = get_append_fd(USERS_FILE);
    int valid = 0;
    int user_added_successfully;

    do
    {
        recv_msg(sockfd, buff);
        printf("RECEIVED FROM CLIENT: %s", buff);
        user_added_successfully = add_user(fd, buff);
        printf("STATUS: %d", user_added_successfully);

        if (user_added_successfully)
        {
            write(sockfd, "1", sizeof(buff));
            valid = 1;
        }
        else
        {
            printf("User adding failed!");
            write(sockfd, "0", sizeof(buff));
        }

    } while (!valid);

}
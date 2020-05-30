#include "user_authentication.c"


void communicate(int sockfd);

void input_validation(int sockfd, char *buff, int (*validation_func)(char *));

int init_input_is_valid(char *input);

void validate_registration(int sockfd, char *buff);

int username_is_valid(char *username);


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
        printf("LOGIN\n");
    }
    // printf("OUTTTT");

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

void input_validation(int sockfd, char *buff, int (*validation_func)(char *))
{
    int valid = 0;
    bzero(buff, MAX);

    printf("CLIENT: SOCKET: %d", sockfd);
    do
    {
        recv_msg(sockfd, buff);

        if (validation_func(buff))
        {
            printf("Server: Response 1\n");
            write(sockfd, "1", sizeof(buff));
            bzero(buff, MAX);
            strcpy(buff, "1");
            valid = 1;
        } 
        else
        {
            printf("Server: Response 0\n");
            write(sockfd, "0", sizeof(buff));
            bzero(buff, MAX);
            strcpy(buff, "1");
        }

    } while (!valid);
}

int init_input_is_valid(char *input)
{
    printf("INIT INPUT::: %s\n", input);

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

    if (user_added_successfully)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    // return user_added_successfully;
}
#include "user_authentication.c"


void communicate(int sockfd);

void input_validation(int sockfd, char *buff, int (*validation_func)(char *));

int init_input_is_valid(char *input);

void validate_registration(int sockfd, char *buff);

int username_is_valid(char *username);

int login_is_valid(char *username);


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

    if (user_added_successfully)
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
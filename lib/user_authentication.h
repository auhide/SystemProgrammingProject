#include <stdio.h>
#include <unistd.h>

#include "base/constants.h"

void send_msg(int sockfd, char *buff);

void recv_msg(int sockfd, char *buff);

char *get_users_storage();

int add_user(int file_desc, char *username);

int validate_user(int file_desc, char *username);


char *get_users_storage()
{
    char *users_data = malloc(sizeof(char) * START_SIZE);
    int users_data_size;
    
    int fd = get_read_fd(USERS_FILE);
    int current_max = START_SIZE;

    int i = 0;
    while (read(fd, &users_data[i], 1) == 1)
    {
        if (current_max < i)
        {
            current_max = i;
            users_data = realloc(users_data, current_max + CHUNK_MEMORY);
        }
        i++;
    }
    close(fd);

    return users_data;
}

int validate_user(int file_desc, char *username)
{
    // strcat(username, "\n");
    if (substring_in_string(username, get_users_storage()))
    {
        return 1;    
    }
    else
    {
        return 0;
    }
    
}

int add_user(int file_desc, char *username)
{
    int is_registered = validate_user(file_desc, username);

    if (!is_registered && 
        strlen(username) >= 4 && 
        strlen(username) < 8 &&
        !substring_in_string(" ", username) &&
        username != NULL)
    {
        write(file_desc, username, strlen(username));
        write(file_desc, "\n", 1);
        // printf("You have successfully been registered, %s\n", username);
        
        return 1;
    }
    else
    {
        // printf("You have already been registered\n");

        return 0;
    }
}

void send_msg(int sockfd, char *buff)
{
    // int n = 0;

    bzero(buff, sizeof(buff));
    scanf("%s", buff);

    write(sockfd, buff, sizeof(buff));
}

void send_cust_msg(int sockfd, char *string)
{
    write(sockfd, string, sizeof(string));
}

void recv_msg(int sockfd, char *buff)
{
    // printf("RECV BEFORE CLEANING: %s\n", buff);
    bzero(buff, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    // printf("READ MESSAGE: %s\n", buff);
}
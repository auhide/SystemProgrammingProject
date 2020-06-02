#include <stdio.h>
#include <unistd.h>

#include "base/constants.h"

void send_msg(int sockfd, char *buff);

void recv_msg(int sockfd, char *buff);

char *get_users_storage(pthread_mutex_t lock);

int add_user(int file_desc, char *username, pthread_mutex_t lock);

int validate_user(int file_desc, char *username, pthread_mutex_t lock);


char *get_users_storage(pthread_mutex_t lock)
{
    char *users_data = malloc(sizeof(char) * START_SIZE);
    int users_data_size;
    
    int fd          = get_read_fd(USERS_FILE);
    int current_max = START_SIZE;

    int i = 0;

    // Acquiring the Lock
    pthread_mutex_lock(&lock);
    while (read(fd, &users_data[i], 1) == 1)
    {
        if (current_max < i)
        {
            current_max = i;
            users_data = realloc(users_data, current_max + CHUNK_MEMORY);
        }
        i++;
    }
    pthread_mutex_unlock(&lock);
    ///////////////////////////

    close(fd);

    return users_data;
}

int validate_user(int file_desc, char *username, pthread_mutex_t lock)
{
    if (substring_in_string(username, get_users_storage(lock)))
    {
        return 1;    
    }
    
    return 0;
}

int add_user(int file_desc, char *username, pthread_mutex_t lock)
{
    int is_registered = validate_user(file_desc, username, lock);

    if (!is_registered && 
        strlen(username) >= 4 && 
        strlen(username) < 8 &&
        !substring_in_string(" ", username) &&
        username != NULL)
    {
        write(file_desc, username, strlen(username));
        write(file_desc, "\n", 1);
        
        return 1;
    }

    return 0;
}

void send_msg(int sockfd, char *buff)
{

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
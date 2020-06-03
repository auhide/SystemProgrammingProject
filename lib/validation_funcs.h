

int username_is_valid(char *username, pthread_mutex_t lock);

int login_is_valid(char *username, pthread_mutex_t lock);

int trips_menu_choice_is_valid(char *choice, pthread_mutex_t lock);

int validate_destination(char *dest, pthread_mutex_t lock);

int validate_coordinate(char *coord, pthread_mutex_t lock);

int validate_distance(char *dist, pthread_mutex_t lock);

int validate_speed(char *dist, pthread_mutex_t lock);

void input_validation(int sockfd, char *buff, pthread_mutex_t lock, int (*validation_func)(char *, pthread_mutex_t));

int init_input_is_valid(char *input, pthread_mutex_t lock);



void input_validation(int sockfd, char *buff, pthread_mutex_t lock, int (*validation_func)(char *, pthread_mutex_t))
{
    int valid = 0;
    bzero(buff, MAX);

    do
    {
        recv_msg(sockfd, buff);

        if (validation_func(buff, lock))
        {
            write(sockfd, "1", sizeof(buff));
            valid = 1;
        } 
        else
        {
            write(sockfd, "0", sizeof(buff));
            bzero(buff, MAX);
            strcpy(buff, "0");  
        }

    } while (!valid);
}


int username_is_valid(char *username, pthread_mutex_t lock)
{
    int fd = get_append_fd(USERS_FILE);
    int user_added_successfully;

    user_added_successfully = add_user(fd, username, lock);
    close(fd);

    if (user_added_successfully && 
        strlen(username) > 3 && 
        strlen(username) < 9)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}


int init_input_is_valid(char *input, pthread_mutex_t lock)
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


int login_is_valid(char *username, pthread_mutex_t lock)
{
    int fd = get_read_fd(USERS_FILE);
    int user_is_registered = validate_user(fd, username, lock);

    if (user_is_registered)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int trips_menu_choice_is_valid(char *choice, pthread_mutex_t lock)
{
    if (substring_in_string(choice, TRIPS_MENU_CHOICES) && 
        !substring_in_string("~", choice))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int validate_destination(char *dest, pthread_mutex_t lock)
{
    if (string_is_alphabetic(dest))
    {
        return 1;
    }
    
    return 0;
}


int validate_coordinate(char *coord, pthread_mutex_t lock)
{
    if (regex_match(coord, COORD_REGEX))
    {
        return 1;
    }

    return 0;
}


int validate_distance(char *dist, pthread_mutex_t lock)
{
    if (regex_match(dist, DIST_REGEX))
    {
        return 1;
    }

    return 0;
}



int validate_speed(char *speed, pthread_mutex_t lock)
{
    if (regex_match(speed, SPEED_REGEX))
    {
        return 1;
    }

    return 0;
}
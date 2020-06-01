#include <stdio.h>


int cmp_distances(char *f_entry, char *s_entry);

int display_top(char *type, int x, char user_data[TRIPS_ROWS][TRIPS_COLS], int len);

int count_user_trips(char user_data[TRIPS_ROWS][TRIPS_COLS], int general_len);

void sort_by_distances(char user_data[TRIPS_ROWS][TRIPS_COLS], int len);

void swap(char *line1, char *line2);


// int main()
// {
//     char *data = malloc(sizeof(char) * START_SIZE);
//     int data_size;

//     get_full_storage(data);
//     data_size = strlen(data);
    
//     int lines_count = get_lines_number(data, data_size);

//     char *lines_arr[TRIPS_COLS] = {0};
//     split_string_to_array(data, STORAGE_DELIMITER, lines_arr);

//     // Dinamically allocate the 2d Array for the users' information
//     char **user_data = (char **)calloc(data_size, sizeof (char *));
//     add_chunks_to(user_data, lines_count);

//     get_user_trips(lines_arr, lines_count, "User3", user_data);
//     // printf("%d", cmp_distances(lines_arr[0], user_data[5]));
//     int user_trips_count = count_user_trips(user_data, lines_count);

//     sort_by_distances(user_data, lines_count);
//     display_top("shortest", 3, user_data, user_trips_count);
    
//     free_2d_array(user_data, lines_count);
//     free(data);

//     return 0;
// }

int cmp_distances(char *f_entry, char *s_entry)
{
    char *f_entry_temp = malloc(sizeof(char) * TRIPS_COLS);
    char *s_entry_temp = malloc(sizeof(char) * TRIPS_COLS);
    strcpy(f_entry_temp, f_entry);
    strcpy(s_entry_temp, s_entry); 

    char f_entry_arr[TRIPS_ROWS][TRIPS_COLS];
    split_string_to_array(f_entry_temp, LINE_DELIMITER, f_entry_arr);

    char s_entry_arr[TRIPS_ROWS][TRIPS_COLS];
    split_string_to_array(s_entry_temp, LINE_DELIMITER, s_entry_arr);

    int num1 = atoi(f_entry_arr[DISTANCE]);
    int num2 = atoi(s_entry_arr[DISTANCE]);

    return num1 - num2;
}

int count_user_trips(char user_data[TRIPS_ROWS][TRIPS_COLS], int general_len)
{
    int trips_count = 0;

    for (int i = 0; i < general_len; i++)
    {
        if (strlen(user_data[i]) > 0) { trips_count++; }
    }

    return trips_count;
}

void sort_by_distances(char user_data[TRIPS_ROWS][TRIPS_COLS], int len)
{
    for (int i = 0; i < len - 1; i++)
    {
        for(int j = 0; j < len - i - 1; j++)
        {
            if (cmp_distances(user_data[j], user_data[j+1]) < 0)
            {
                swap(user_data[j], user_data[j+1]);
            }
        }
    }
}

void swap(char *line1, char *line2)
{
    char *temp_line = malloc(sizeof(char) * TRIPS_COLS);
    strcpy(temp_line, line1);

    strcpy(line1, line2);
    strcpy(line2, temp_line);
}


int display_top(char *type, int x, char user_data[TRIPS_ROWS][TRIPS_COLS], int len)
{
    char *curr_line[TRIPS_COLS];

    printf("T O P - %d\n", x);
    if (len >= x)
    {
        if (strcmp(type, "longest") == 0)
        {
            for (int i = 0; i < x; i++)
            {
                split_string_to_array(user_data[i], LINE_DELIMITER, curr_line);
                if (curr_line[0] != NULL)
                {
                    print_line_arr(curr_line);
                }
            }
        } 
        else
        {
            for (int i = x - 1; i >= 0; i--)
            {
                split_string_to_array(user_data[i], LINE_DELIMITER, curr_line);
                if (curr_line[0] != NULL)
                {
                    print_line_arr(curr_line);
                }
            }
        }

        return 1; 
    }
    else
    {
        printf("Your trips are less than %d, they are %d", x, len);
        return 0;
    }
}
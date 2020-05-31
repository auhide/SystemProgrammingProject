// Base constants
#define START_SIZE 256
#define LINE_SIZE 200
#define CHUNK_MEMORY 5
#define STORAGE_DELIMITER "\n"
#define LINE_DELIMITER "#"
#define EXIT "exit"

// Trips constants
#define TRIPS_ROWS 8
#define TRIPS_COLS 1024
#define STORAGE "data/storage.txt"
#define USER 0
#define DESTINATIONS 1
#define LATITUDE1 2
#define LONGITUDE1 3
#define LATITUDE2 4
#define LONGITUDE2 5
#define AVG_SPEED 6
#define DISTANCE 7
#define TRIPS_MENU_CHOICES "1~2~3~4"

// Regexes
#define COORD_REGEX "^([0-9]+\\.[0-9]{3})$"
#define SPEED_REGEX "^([0-9]{1,3})$"
#define DIST_REGEX "^([0-9]{1,7})$"

// User authentication constants
#define MAX 1024
#define USERS_FILE "/home/auhide/Desktop/SPrProject/data/users.txt"
#define INIT_VALID_INPUTS "1~2\n"
#define REGISTRATION "1"
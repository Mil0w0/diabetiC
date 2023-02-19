typedef struct Config Config;
struct Config {
    char *username;
    char *password;
    char *unit;
};

Config *readFile(char *filename);
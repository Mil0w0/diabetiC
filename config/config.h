typedef struct Config Config;
struct Config {
    char *username;
    char *password;
};

Config *readFile(char *filename);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

Config *readFile(char *filename)
{

    Config *config = malloc(sizeof(Config));
    config->username = malloc(30);
    config->password = malloc(30);

    config->username = "Empty";
    config->password = "Empty";

    char line[256];
    // char tempUser[30];
    // char tempPass[30];
    char *key;
    const char delim[2] = "=";

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '#')
            continue;
        
        key = strtok(line, delim);
        while(key != NULL)
        {
            if(key[0] != '-')
            {
                if(config->username == "Empty")
                {
                    int size = strlen(key);
                    config->username = malloc(size);
                    memcpy(config->username, key, size-1);
                    config->username[size-1] = '\0';
                }
                else if(config->password == "Empty")
                {
                    int size = strlen(key);
                    config->password = malloc(size+1);
                    memcpy(config->password, key, size);
                    config->password[size] = '\0';
                }
            }
            key = strtok(NULL, delim);
        }
    }
    fclose(file);

    return config;
}
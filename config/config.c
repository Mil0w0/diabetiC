#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

Config *readFile(char *filename)
{

    Config *config = malloc(sizeof(Config));

    config->username = "Empty";
    config->password = "Empty";

    char line[256];
    char tempUser[30];
    char tempPass[30];
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
                    strcpy(tempUser, key);
                    config->username = tempUser;
                }
                else if(config->password == "Empty")
                {
                    strcpy(tempPass, key);
                    config->password = tempPass;
                }
            }
            key = strtok(NULL, delim);
        }
    }
    fclose(file);

    return config;
}
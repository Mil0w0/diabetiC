#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void readFile(char *filename)
{
    char line[256];

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    fgets(line, 256, file);
    puts(line);
    fclose(file);
}
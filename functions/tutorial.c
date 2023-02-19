#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int printTutorial(){
    if (readTutorial() == 1){
        return 1;
    }
    return 0;
}

int readTutorial(){

    char line[256];
    FILE *file = fopen("README.md", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    printf("\n");
    fclose(file);
    return 0;
}
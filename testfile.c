#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;
    char * variable = "accueil";
    int varLong = strlen(variable);

    fp = fopen("test.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        int i;
        int equal = 1;
        char *valVariable = "";
        for (i=0;i!=varLong;i++) {
            if(read[i]!=variable[i]) {
                equal = 0;
            }
            if(equal!=0) {
                // Pour passer le point-virgule :
                fseek(fp,1,SEEK_CUR);
                while(read+1) {
                    strcat(fgetc(fp),valVariable);
                }
                return valVariable;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}
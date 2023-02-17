// This file handles the manipulation about the glycemias.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sqlite3.h"
#include "../users/users.h"
#include "glycemia.h"

//text variables to be put in an env file later
char defaultUnit[] = "Default glycemia unit is g/L.\n";
char askForGlycemia[] = "Enter current glycemia : ";

//get glycemia from user
double inputsGlycemia(){
   char *eptr;
   double glycemia = -1.0;
   double irrealMax = 30; //but can be different if unit is different.

    do
    {   
        char tempGlycemia[10];
        printf("%s", defaultUnit);
        printf("%s", askForGlycemia);
        scanf("%s", &tempGlycemia);
        strcat(tempGlycemia, "\0");

        //Verification and convert input to double
        if (!checkGlycemia(tempGlycemia)){
            printf("\n"); 
            printf("Targeted glycemia must be numeric !\n\n");
        } else {
            glycemia = strtod(tempGlycemia, &eptr);  
        }

    } while (glycemia <= 0 || glycemia > irrealMax);

    return glycemia;
}
char * inputComment(){}


//test code
// int main(int argc, char **argv){
//     double glycemia = inputsGlycemia();
//     printf("%.2lf", glycemia);
// }
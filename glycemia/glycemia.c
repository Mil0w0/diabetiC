// This file handles the manipulation about the glycemias.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sqlite3.h"
#include "glycemia.h"

//text variables to be put in an env file later
char defaultUnit[] = "Default glycemia unit is g/L.\n";
char askForGlycemia[] = "Enter current glycemia : ";

//get glycemia from user
double inputsGlycemia(){
   char tempGlycemia;
   double glycemia;
   double irrealMax = 100; //but can be different if unit is different.

    do
    {
        printf("%s", defaultUnit);
        printf("%s", askForGlycemia);
        scanf("%lf", &glycemia);
        printf("\n");
    } while (glycemia < 0 || glycemia > irrealMax);
    //need to write double with a dot not comma. how do we check that ? strchr() if it s a string;
    //so what if everything is a string and we cast it to double when needed

    return glycemia;
}
char * inputComment(){}


//test code
// int main(int argc, char **argv){
//     double glycemia = inputsGlycemia();
//     printf("%.2lf", glycemia);
// }
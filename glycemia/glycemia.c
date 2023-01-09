// This file handles the manipulation about the glycemias.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glycemia.h"

//text variables to be put in an env file later
char defaultUnit[] = "Default glycemia unit is g/L.\n";
char askForGlycemia[] = "Enter current glycemia : ";

//get glycemia from user
double inputsGlycemia(){
    double glycemia;
    double irrealMax = 100; //but can be different if unit is different.
    do {
        printf(defaultUnit);
        printf(askForGlycemia);
        scanf("%lf", &glycemia);
        printf("\n");
    } while (glycemia < 0 || glycemia > irrealMax);
    //need to write double with a dot not comma. how do we check that ?

    return glycemia;
}


//test code
int main(int argc, char **argv){
    double glycemia = inputsGlycemia();
    printf("%.2lf", glycemia);
}
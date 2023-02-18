// This file handles the manipulation about the glycemias.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sqlite3.h"
#include "../users/users.h"
#include "../functions/functions.h"
#include "glycemia.h"
#include "../entries/entry.h"


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
char * inputComment(){
    int maxChars = 255;
    char *askForComment = "Any comments ? Leave empty if none.\n";
    printf("%s\n", askForComment);

    char *input = malloc(maxChars); 
    fflush(stdin);
    char *gettingInput = fgets(input, maxChars, stdin);

    if (gettingInput == NULL){
        printf("\nCouldn't get input correctly.\n");
    }

    return input;
  
}

double averageGlycemia(Entry *n){
   double sum = 0;
   int count = 0;
   double average;

   while(n){
      sum += n->value ;
      count++;
      n = n->next;
   }
   average = sum/count;

   printf("Your average glycemia is %.2lf g/L for a total of %d glycemia logs.\n", average, count);

   return (average < 0) ? -1 : average;
}
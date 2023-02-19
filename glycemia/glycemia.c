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
   int unit = 1; // 1 = g/L
   double glycemia = -1.0;
   double irrealMax = 30; //but can be different if unit is different.

    do
    {   
        char tempGlycemia[10];
        printf("%s", defaultUnit);
        printf("%s", askForGlycemia);
        scanf("%s", &tempGlycemia);
        strcat(tempGlycemia, "\0");
        getchar(); //Otherwise there's a \n in the buffer.

        //Verification and convert input to double
        if (!checkGlycemia(tempGlycemia)){
            printf("\n"); 
            printf("Glycemia must be numeric !\n\n");
        } else {
            glycemia = strtod(tempGlycemia, &eptr);  
        }

    } while (glycemia <= 0 || glycemia > irrealMax);

   alertGlycemiaOutOfRange(glycemia,unit);
    return glycemia;
}

char * inputComment(){
    int maxChars = 255;
    char askForComment[] = "Any comments ? Leave empty if none.\n";
    printf("%s", askForComment);

    char *input = malloc(maxChars);

    char *gettingInput = fgets(input, maxChars, stdin);

    if (gettingInput == NULL){
        printf("\nCouldn't get input correctly.\n");
    }

    //When there's \n at the end of string we remove it.
    char *lastchar = strrchr(input,'\n');
    if (lastchar){
        *lastchar = '\0';
    }

    int size = strlen(input) == 0 ? 1 : strlen(input);
    input = realloc(input,size);
    input[size] = '\0';

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

   printf("Your average glycemia is %.2lf g/L for a total of %d glycemia logs.\n\n", average, count);

   return (average < 0) ? -1 : average;
}

void alertGlycemiaOutOfRange(double glycemia, int unit){
   char *hyperAlert = "You are in hyperglycemia ! You might want to put more insulin but be careful.\n";
   char *hypoAlert = "You are in hypoglycemia ! Eat some fast carbs like 3 sugars (15g of carbs)\n";
   char *severeHypoAlert = "You are in severe hypoglycemia. It can be life-threatening. Get help.\n";

   //IF UNIT == 1 THEN UNIT IS g/L and
   double max = (unit == 1) ? 1.80 : 180;
   double low = (unit == 1) ? 0.70 : 70;
   double min = (unit == 1) ? 0.50 : 50;

   if (glycemia >= max)
   {
      printf("%s", hyperAlert);
   }
   else if (glycemia <= min)
   {
      printf("%s", severeHypoAlert);
   }
   else if (glycemia <= low)
   {
      printf("%s", hypoAlert);
   }
}



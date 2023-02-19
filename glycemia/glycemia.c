// This file handles the manipulation about the glycemias.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sqlite3.h"
#include "../users/users.h"
#include "../functions/functions.h"
#include "glycemia.h"
#include "../entries/entry.h"

char askForGlycemia[] = "Enter current glycemia : ";

int whichUnit(char unit[])
{
    if (unit[0] == '0'){
        return 0;
    } else if (unit[0] == '1'){
            return 1;
    } else if (unit[0] == '2'){
        return 2;
    } else
    {
        return 0; // g/L  by default
    }
}

//get glycemia from user
double inputsGlycemia(char unit[]){
   char *eptr;
   char *defaultUnit = malloc(35);
   double glycemia = -1.0;
   double irrealMax = 30;
   int theUnit = whichUnit(unit);

    switch (theUnit) {
        case 0:
            defaultUnit = strcpy(defaultUnit,"Default glycemia unit is g/L.\n");
            irrealMax = 30;
            break;
        case 1:
            char text[] = "Default glycemia unit is mg/dL.\n";
            defaultUnit = realloc(defaultUnit, strlen(text));
            defaultUnit = strncpy(defaultUnit,"Default glycemia unit is g/L.\n", strlen(text));
            irrealMax = 3000;
            break;
        case 2:
            char text2[] = "Default glycemia unit is mmol/L.\n";
            "Default glycemia unit is mg/dL.\n";
            defaultUnit = realloc(defaultUnit, strlen(text2));
            defaultUnit = strncpy(defaultUnit,"Default glycemia unit is g/L.\n", strlen(text2));
            irrealMax = 166;
        default:
            irrealMax = 30;
            break;
    }

    do
    {   
        char tempGlycemia[5];
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

    alertGlycemiaOutOfRange(glycemia, theUnit);
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
   char hyperAlert[] = "You are in hyperglycemia ! You might want to put more insulin but be careful.\n";
   char hypoAlert[] = "You are in hypoglycemia ! Eat some fast carbs like 3 sugars (15g of carbs)\n";
   char severeHypoAlert[] = "You are in severe hypoglycemia. It can be life-threatening. Get help.\n";

   //UNIT == 0 : g/L | UNIT == 1 : g/L | UNIT == 2 : mmol/L
   double max = (unit == 0) ? 1.80 : ( (unit == 2) ? 9.99 : 180) ;
   double low = (unit == 0) ? 0.70 : ( (unit == 2) ? 3.89 : 70 ) ;
   double min = (unit == 0) ? 0.50 : ( (unit == 2) ? 2.78 : 50 ) ;

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



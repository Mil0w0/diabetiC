// This file handles the creation of the chained list of entries;
//gcc -o entries/entry.c glycemia/glycemia.c    

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "sqlite3.h"
#include "entry.h"
#include "../glycemia/glycemia.h"


//Create a node 
Entry *createEntry(double value, char *comment, char *date, int position){
    Entry *glycemia = malloc(sizeof(Entry)); 
    glycemia->next = NULL;
    glycemia->value = value;
    if (comment != NULL) //if the user put a comment 
    {
        int size = strlen(comment);
        glycemia->comment = malloc(size);
        memcpy(glycemia->comment, comment, size);
    }
    glycemia->entries = position;

    return glycemia;
}

//Add an entry to the diary : a chained list of nodes
void addEntry(Entry *lastEntry, double i, char *comment, char *date, int position){
    
    //if user add a new entry, position is calculated from the last entry
    //if we are recreating the list from the db data then position has already been calculated.
    if (position == 0){
         while(lastEntry->next){ //looking for the current last entry
            lastEntry = lastEntry->next;
        }
        position = (lastEntry->entries + 1) ; 
    }
    lastEntry->next = createEntry(i, comment, date, position);
}

// modify the content of a node     

//does data exist about this user ? 

int main(int argc, char **argv){

    //If user enters new information use inputs Function 
    //Else if user is login in and we wanna access the past data use data from database.

    Entry *n = createEntry(inputsGlycemia(), "comment", NULL, 1);
    addEntry(n, inputsGlycemia(), "yes", NULL, 0); 

    printf("%.2lf\n", n->value);
    printf("%d\n", n->entries);
    printf("%s\n", n->comment);

    printf("%.2lf\n",n->next->value);
    printf("%d\n",n->next->entries);
    printf("%s\n", n->next->comment);

    
    //Freeing everything before exiting
    while(n){
        printf("%d\n", n->entries);
        Entry * tmp = n->next;
        free(n->comment);    
        free(n); 
        n = tmp;
    }
   
            
    return 0;
}

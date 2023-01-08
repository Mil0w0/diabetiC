// This file contains work about the glycemia ? 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "sqlite3.h"
#include "glycemia.h"


//Create a node
Entry *createEntry(double value, char *comment, int position){
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
void addEntry(Entry *firstEntry, double i, char *comment){
    while(firstEntry->next){ //looking for the current last entry
        firstEntry = firstEntry->next;
    }
    firstEntry->next = createEntry(i, comment, (firstEntry->entries + 1));
}

// modify the content of a node     

//delete a node - do we need this ?
// void pop(Entry *n){
//     n->next = n;
// }

int main(int argc, char **argv){

    Entry *n = createEntry(5.2, "comment", 1);
    addEntry(n, 7.0, "yes");

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

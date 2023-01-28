// This file handles the creation of the chained list of entries;
//gcc -o entries/entry.exe entries/entry.c glycemia/glycemia.c -lpthread -ldl -lm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
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

//send the entry to be keep in database

int sendEntryToDatabase(Entry *glycemia){
   sqlite3 *db;
   char *err_msg = 0;
   sqlite3_stmt *res;

   int rc = sqlite3_open("test.db", &db);

   if (rc != SQLITE_OK) {
      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
      }

   char *sql = "INSERT INTO GLYCEMIA(VALUE) VALUES (:value)";
   //we are not using the _v3 version cuz we don't need the special prepflags argumentw
   rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

   //Replacing the named parameters with the value we want
   if (rc == SQLITE_OK) {
      //we save the position of the named parameter to use it later.
      int idx = sqlite3_bind_parameter_index(res, ":value");
      //if it was an int we would be using sqlite3_bind_int(), etc.
      sqlite3_bind_double(res, idx, glycemia->value);
   } else {
      //Error hzndling
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
   }

   rc = sqlite3_step(res);

   //Error hzndling
   if (rc != SQLITE_DONE) {
      printf("execution failed: %s", sqlite3_errmsg(db));
   }

   //Finishes the request, returns SQLITE_OK if all good if we don't do it can lead to segfaults
   sqlite3_finalize(res);

   printf("Insert of double successfully done.\n");
   sqlite3_close(db);
}
int main(int argc, char **argv){

    //If user enters new information use inputs Function gt
    //Else if user is login in and we wanna access the past data use data from database.

    Entry *n = createEntry(inputsGlycemia(), "comment", NULL, 1);
    addEntry(n, inputsGlycemia(), "yes", NULL, 0); 

    printf("%.2lf\n", n->value);
    printf("%d\n", n->entries);
    printf("%s\n", n->comment);

    printf("%.2lf\n",n->next->value);
    printf("%d\n",n->next->entries);
    printf("%s\n", n->next->comment);

    sendEntryToDatabase(n);

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

//Compilation on linux : gcc main.c sqlite3.c -o main.exe -lpthread -ldl -lm
//To compile : gcc main.c sqlite3.c database/database.c entries/entry.c glycemia/glycemia.c -o main.exe

#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include "database/database.h"
#include "entries/entry.h"
#include "glycemia/glycemia.h"

int main(int argc, char **argv) {
   int user_id = 1;

   createDatabase();

   //TESTING
   Entry *n = createEntry(inputsGlycemia(), "comment", NULL, 1, user_id);
   addEntry(n, inputsGlycemia(), "yes", NULL, 0, user_id);

   printf("%.2lf\n", n->value);
   printf("%d\n", n->entries);
   printf("%s\n", n->comment);

   printf("%.2lf\n",n->next->value);
   printf("%d\n",n->next->entries);
   printf("%s\n", n->next->comment);

   sendEntryToDatabase(n);
   getGlycemiaDataFromDB(1);
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
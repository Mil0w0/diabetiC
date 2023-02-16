// This file handles the creation of the chained list of entries;
//gcc -o entries/entry.exe entries/entry.c glycemia/glycemia.c sqlite3.c -lpthread -ldl -lm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../sqlite3.h"
#include "entry.h"
#include "../glycemia/glycemia.h"

//Create a node 
Entry *createEntry(double value, char *comment, char *date, int position, int user_id){
    Entry *glycemia = malloc(sizeof(Entry)); 

    glycemia->next = NULL;
    glycemia->value = value;
    glycemia->entries = position;
    glycemia->user_id = user_id;
    
    if (comment != NULL) //if the user put a comment 
    {
        int size = strlen(comment);
        glycemia->comment = malloc(size);
        memcpy(glycemia->comment, comment, size);
    }
    if (date != NULL) //if the user put a comment 
    {
        int size = strlen(date);
        glycemia->taken_at = malloc(size);
        memcpy(glycemia->taken_at, date, size);
    }

    return glycemia;
}

//Add an entry to the diary : a chained list of nodes
void addEntry(Entry *lastEntry, double i, char *comment, char *date, int position, int user_id){
    
    //if user add a new entry, position is calculated from the last entry
    //if we are recreating the list from the db data then position has already been calculated.
    if (position == 0){
         while(lastEntry->next){ //looking for the current last entry
            lastEntry = lastEntry->next;
        }
        position = (lastEntry->entries + 1) ; 
    }
    lastEntry->next = createEntry(i, comment, date, position, user_id);
    //sendEntryToDatabase(lastEntry->next);
}

// modify the content of a node     

//send the entry to be kept in database
int sendEntryToDatabase(Entry *glycemia){

   char successfullySaved[] = "\nYour glycemia has been saved into the database.\n";

   sqlite3 *db;
   char *err_msg = 0;
   sqlite3_stmt *res;

   int rc = sqlite3_open("database/diabetic.db", &db);

   if (rc != SQLITE_OK) {
      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
      }

   char *sql = "INSERT INTO GLYCEMIA(VALUE, COMMENT, USER_ID) VALUES (:value, :comment, :user_id)";
   //we are not using the _v3 version cuz we don't need the special prepflags argumentw
   rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

   //Replacing the named parameters with the value we want
   if (rc == SQLITE_OK) {
      int parameter = sqlite3_bind_parameter_index(res, ":value");
      sqlite3_bind_double(res, parameter, glycemia->value);

      parameter =  sqlite3_bind_parameter_index(res, ":comment");
      sqlite3_bind_text(res, parameter, glycemia->comment, strlen(glycemia->comment), NULL);

      parameter =  sqlite3_bind_parameter_index(res, ":user_id");
      sqlite3_bind_int(res, parameter, glycemia->user_id);

   } else {
      //Error handling
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
   }

   rc = sqlite3_step(res);

   //Error hzndling
   if (rc != SQLITE_DONE) {
      printf("execution failed: %s", sqlite3_errmsg(db));
   }

   //Finishes the request, returns SQLITE_OK if all good if we don't do it can lead to segfaults
   sqlite3_finalize(res);

   printf("%s", successfullySaved);
    ;
}

// static int callback(void *NotUsed, int argc, char **argv, char **azColName)
// {
//    int i;
//    for (i = 0; i < argc; i++)
//    {
//       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
// }

//return -1 if can't open db, 0, if all good, else if empty
//return the entries
Entry *getGlycemiaDataFromDB(unsigned int user_id){
   int emptyLogs = 0;
   Entry *firstGlycemia;
   char *emptyLogsText = "No logs registered yet.\n";
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   sqlite3_stmt *res;

   rc = sqlite3_open("database/diabetic.db", &db);

   if (rc != SQLITE_OK) {
      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
   }

   //binding parameters fails/
   //char date_format[] = "%d/%m/%Y, %H:%M";
   
   char *sql = "SELECT id, value, STRFTIME(\'%d/%m/%Y\', taken_at), comment FROM GLYCEMIA WHERE GLYCEMIA.user_id = :user_id";
   rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

   if (rc == SQLITE_OK) {
      int parameter = sqlite3_bind_parameter_index(res, ":user_id");
      sqlite3_bind_int(res, parameter, user_id);

      //parameter = sqlite3_bind_parameter_index(res, ":date_format");
      //sqlite3_bind_text(res, parameter, date_format, strlen(date_format), NULL);
   }
   else
   {
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
   }

   // CREATE GLYCEMIA LOGS FROM DATABASE

   if (sqlite3_step(res) != SQLITE_ROW){
      printf("%s\n",emptyLogsText);
      emptyLogs = 1;
   }
   else
   {      
      //create the glycemia log struct
      firstGlycemia = createEntry(sqlite3_column_double(res, 1),
                  sqlite3_column_text(res, 3),
                  sqlite3_column_text(res, 2),
                  sqlite3_column_int(res, 0),
                  user_id
                  );
      //printGlycemiaLog(res);
   }

   while (rc = sqlite3_step(res) == SQLITE_ROW) 
   {
      //add the glycemia log node to the chained list
      addEntry(firstGlycemia,
               sqlite3_column_double(res, 1),
               sqlite3_column_text(res, 3),
               sqlite3_column_text(res, 2),
               sqlite3_column_int(res, 0),
               user_id
               );
      //printGlycemiaLog(res);
   }
   
   sqlite3_finalize(res);
   return firstGlycemia;
}

//à partir de la struct pas de ça.
void printGlycemiaLog(sqlite3_stmt *res){
   char *glycemiaLogsTitle = "--------All your glycemia logs--------\n";

   //printf("\n%s\n", glycemiaLogsTitle);
   printf(" ----ID:%d----\n", sqlite3_column_int(res, 0));
   printf("| Value      : %.2lf g/L\n", sqlite3_column_double(res, 1));
   printf("| Date       : %s\n", sqlite3_column_text(res, 2));
   printf("| Comment    : %s\n", sqlite3_column_text(res, 3));
   printf(" ------------\n\n");
}

void showEntries(Entry *glycemia) {
   char *emptyLogsText = "No logs registered yet.\n";
   char *glycemiaLogsTitle = "--------All your glycemia logs--------\n";
    
   if (glycemia == NULL)
   {
      printf("%s",emptyLogsText);
   } else 
   {
      printf("\n%s\n", glycemiaLogsTitle);
   }
   
   while(glycemia)
   {
      printf("----ID:%d----\n", glycemia->entries);
      printf("| Value      : %.2lf g/L\n",glycemia->value);
      printf("| Date       : %s\n",glycemia->taken_at);
      printf("| Comment    : %s\n",glycemia->comment);
      printf(" ------------\n\n");

      glycemia = glycemia->next;
   }
   
}

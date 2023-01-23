//Compilation on linux : gcc main.c sqlite3.c -o main.exe -lpthread -ldl -lm
//To compile : gcc main.c sqlite3.c -o main.exe
// THIS FILE IS ONLY A TEST AND IT SEEMS TO BE WORKING 

#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char **argv){

    sqlite3 *db;
     char *zErrMsg = 0;
   int rc;
   char *sql;


   //CREATE  TEST DATABASE;
   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

    //CREATE A USER TABLE;
     /* Create SQL statement */
    sql = "CREATE TABLE USER("  \
      "ID               INT PRIMARY KEY     NOT NULL," \
      "USERNAME         VARCHAR(30)    NOT NULL," \
      "AGE              INT     NOT NULL," \
      "PASSWORD         VARCHAR(30)," \
      "CREATED_AT       DATETIME);";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }

   sqlite3_close(db);


return 0;
}

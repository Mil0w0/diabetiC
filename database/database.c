//
// Created by milo on 06/02/23.
//

#include <stdlib.h>
#include <stdio.h>
#include "../sqlite3.h"
#include "database.h"


int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i = 0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void createDatabase(sqlite3 *db, char *sql, char *zErrMsg, int rc)
{
   rc = sqlite3_open("database/diabetic.db", &db);

   if( rc )
   {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   } else
   {
      printf("\n");
   }
}

// CREATE A USER TABLE IF NOT EXISTS;
void createTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc)
{
   sql = "CREATE TABLE IF NOT EXISTS USERS("  \
            "ID                INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
            "USERNAME          VARCHAR(30)    NOT NULL," \
            "AGE               INTEGER     NOT NULL," \
            "PASSWORD          VARCHAR(30)," \
            "TARGETED_GLYCEMIA FLOAT     NOT NULL," \
            "HYPERGLYCEMIA     INTEGER     NOT NULL," \
            "HYPOGLYCEMIA      INTEGER     NOT NULL," \
            "CREATED_AT        DATETIME);";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK )
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
}

void printTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc)
{
   sql = "SELECT * FROM USERS";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK )
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
}


void createTableGlycemia(sqlite3 *db, char *sql, char *zErrMsg, int rc){

   sql = "CREATE TABLE IF NOT EXISTS GLYCEMIA("  \
      "ID            INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL," \
      "VALUE         VARCHAR(30)      NOT NULL," \
      "TAKEN_AT      DATETIME         DEFAULT (CURRENT_TIMESTAMP)  NOT NULL," \
      "COMMENT       VARCHAR(255)," \
      "USER_ID       INT              NOT NULL,"\
      "FOREIGN KEY (USER_ID) REFERENCES USERS (ID));";


   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if (rc != SQLITE_OK)
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else
   {
      //fprintf(stdout, "Table GLYCEMIA created successfully\n");
   }
}


void printTableGlycemia(sqlite3 *db, char *sql, char *zErrMsg, int rc)
{
   sql = "SELECT * FROM GLYCEMIA";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK )
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
}

int createFullDatabase(){
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   //Create DB.
   rc = sqlite3_open("database/diabetic.db", &db);

   if (rc)
   {
      fprintf(stderr, "Can't open database diabetic.db : %s\n", sqlite3_errmsg(db));
      return (0);
   }
   else
   {
      fprintf(stderr, "Opened database successfully\n");
   }

   //CREATE A USER TABLE IF NOT EXISTS;
   sql = "CREATE TABLE IF NOT EXISTS USERS("  \
        "ID               INTEGER PRIMARY KEY AUTOINCREMENT     NOT NULL," \
        "USERNAME         VARCHAR(30)    NOT NULL," \
        "AGE              INT     NOT NULL," \
        "PASSWORD         VARCHAR(30)," \
        "CREATED_AT       DATETIME DEFAULT (CURRENT_TIMESTAMP));";

   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK )
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   else
   {
      fprintf(stdout, "Table USERS created successfully\n");
   }

   //CREATE A GLYCEMIA TABLE;
   sql = "CREATE TABLE IF NOT EXISTS GLYCEMIA("  \
      "ID            INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL," \
      "VALUE         VARCHAR(30)      NOT NULL," \
      "TAKEN_AT      DATETIME         DEFAULT (CURRENT_TIMESTAMP)  NOT NULL," \
      "COMMENT       VARCHAR(255)," \
      "USER_ID       INT              NOT NULL,"\
      "FOREIGN KEY (USER_ID) REFERENCES USERS (ID));";


   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if (rc != SQLITE_OK)
   {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else
   {
      //fprintf(stdout, "Table GLYCEMIA created successfully\n");
   }
}
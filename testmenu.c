#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sqlite3.h"
#include <time.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

bool LogIn(char *username, char *password, sqlite3 *db) {
    char *zErrMsg = 0;
    int rc;
    char *sql;

    sql = "SELECT * FROM USERS WHERE USERNAME = username AND PASSWORD = password";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(rc == 0) {
            return false;
        }else if (rc == 1) {
            return true;
        }
    }
}

int main(int argc, char **argv) {

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    int choice;
    int connected = 0;
    char username[30];
    char password[30];
    int age;
    bool isUserValid = false;
    //CREATE  TEST DATABASE;
    rc = sqlite3_open("test.db", &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    //CREATE A USER TABLE IF NOT EXISTS;
    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS USERS("  \
        "ID               INT PRIMARY KEY     NOT NULL," \
        "USERNAME         VARCHAR(30)    NOT NULL," \
        "AGE              INT     NOT NULL," \
        "PASSWORD         VARCHAR(30)," \
        "CREATED_AT       DATETIME);";        
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Print the table
    sql = "SELECT * FROM USERS";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    //INSERT INTO THE USERS TABLE SOME USERS;
    sql = "INSERT INTO USERS (ID, USERNAME, AGE, PASSWORD, CREATED_AT)" \
          "VALUES (1, 'John', 25, '1234', CURRENT_TIMESTAMP);" \
          "INSERT INTO USERS (ID, USERNAME, AGE, PASSWORD, CREATED_AT)" \
          "VALUES (2, 'Paul', 30, '1234', CURRENT_TIMESTAMP);" \
          "INSERT INTO USERS (ID, USERNAME, AGE, PASSWORD, CREATED_AT)" \
          "VALUES (3, 'Ringo', 35, '1234', CURRENT_TIMESTAMP);" \
          "INSERT INTO USERS (ID, USERNAME, AGE, PASSWORD, CREATED_AT)" \
          "VALUES (4, 'George', 40, '1234', CURRENT_TIMESTAMP);" \
          "INSERT INTO USERS (ID, USERNAME, AGE, PASSWORD, CREATED_AT)" \
          "VALUES (5, 'Pete', 45, '1234', CURRENT_TIMESTAMP);";

    do {
        // Case 1: User is not connected
        if(connected == 0) {
            printf("Welcome to your glycemia database, please choose an option:\n");
            printf("1. Log in with your user\n");
            printf("2. Create a new user\n");
            printf("3. Exit\n");
            scanf("%d", &choice);

            if(choice == 1) {
                printf("Please enter your username:\n");
                scanf("%s", &username);
                printf("Please enter your password:\n");
                scanf("%s", &password);
                isUserValid = LogIn(username, password, db); 
                if(isUserValid) {
                    printf("You are now connected!\n");
                    connected = 1;
                }else {
                    printf("Wrong username or password, please try again!\n");
                }
            }else if(choice == 2) {
                printf("Please enter your username:\n");
                scanf("%s", &username);
                printf("Please enter your password:\n");
                scanf("%s", &password);
                printf("Please enter your age:\n");
                scanf("%d", &age);
                //INSERT USER IN DATABASE;
                sql = "INSERT INTO USERS (USERNAME, AGE, PASSWORD, CREATED_AT)" \
                      "VALUES ("username", "age", "password", CURRENT_TIMESTAMP);";

                rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

                if( rc != SQLITE_OK ) {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    fprintf(stdout, "User created successfully\n");
                }

            }else if(choice == 3) {
                printf("Goodbye!\n");
                exit(0);
            }
        // Case 2: User is connected
        }else if(connected == 1) {
            printf("Now, please choose an option:\n");
            printf("1. Add an other glycemia log\n");
            printf("2. See your glycemia logs\n");
            printf("3. See your glycemia logs for a specific date\n");
            printf("4. Log out\n");
            scanf("%d", &choice);
        }
    }while(choice != 3);

    return 0;
}
//Compilation on linux : gcc testmenu.c sqlite3.c -o testmenu.exe -lpthread -ldl -lm
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i = 0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int checkUser(void *NotUsed, int argc, char **argv, char **azColName)
{
    return 1;
}

bool LogIn(char *username, char *password, sqlite3 *db)
{
    char *zErrMsg = 0;
    int rc;
    char *sql;

    sql = "SELECT * FROM USERS WHERE USERNAME = username AND PASSWORD = password";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else 
    {
        if(rc == 0) 
        {
            return false;
        }else if (rc == 1) 
        {
            return true;
        }
    }
}

int main(int argc, char **argv)
{

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    int choice;
    int connected = 0;
    char username[30];
    char password[30];
    char age[2];
    bool isUserValid = false;
    //CREATE  TEST DATABASE;
    rc = sqlite3_open("test.db", &db);

    if( rc )
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else
    {
        printf("\n");
    }

    //CREATE A USER TABLE IF NOT EXISTS;
    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS USERS("  \
        "ID               INTEGER PRIMARY KEY AUTOINCREMENT     NOT NULL," \
        "USERNAME         VARCHAR(30)    NOT NULL," \
        "AGE              INT     NOT NULL," \
        "PASSWORD         VARCHAR(30)," \
        "CREATED_AT       DATETIME);";        
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    // Print the table users for debug only
    // sql = "SELECT * FROM USERS";

    // rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    // if( rc != SQLITE_OK )
    // {
    //     fprintf(stderr, "SQL error: %s\n", zErrMsg);
    //     sqlite3_free(zErrMsg);
    // }

    do 
    {
        // Case 1: User is not connected
        if(connected == 0)
        {
            printf("Welcome to your glycemia database, please choose an option:\n");
            printf("1. Log in with your user\n");
            printf("2. Create a new user\n");
            printf("9. Exit\n");
            scanf("%d", &choice);

            if(choice == 1)
            {
                printf("\nPlease enter your username:\n");
                scanf("%s", &username);
                strcat(username, "\0");
                printf("\n");

                printf("Please enter your password:\n");
                fflush(stdin);
                char* input = malloc(30);
                char* getPassword = malloc(30);
                getPassword = fgets(input, 30, stdin);
                for(int i = 0; i < strlen(getPassword); i++)
                {
                    if(getPassword[i] == '\n')
                    {
                        getPassword[i] = getPassword [i + 1];
                    }
                }
                printf("\n");

                char sql[200] = "SELECT * FROM USERS WHERE username = '";
                strcat(sql, username);
                strcat(sql, "' AND password = '");
                strcat(sql, getPassword);
                strcat(sql, "'");

                rc = sqlite3_exec(db, sql, checkUser, 0, &zErrMsg);

                if( rc == SQLITE_OK )
                {
                    printf("Wrong username or password, please try again\n\n");
                    sqlite3_free(zErrMsg);
                } else
                {
                    printf("You are now connected !\n");
                    printf("Welcome %s ^^\n\n", username);
                    connected = 1;
                }

                free(input);
                free(getPassword);


            }else if(choice == 2)
            {

                char sqltest[200] = "INSERT INTO USERS (USERNAME, PASSWORD, AGE, CREATED_AT) VALUES ('\0";
                printf("\nPlease enter your username:\n");
                scanf("%s", &username);
                printf("\n");
                strcat(username, "\0");

                printf("Please enter your password:\n");
                fflush(stdin);
                char* input = malloc(30);
                char* getPassword = malloc(30);
                getPassword = fgets(input, 30, stdin);
                for(int i = 0; i < strlen(getPassword); i++)
                {
                    if(getPassword[i] == '\n') {
                        getPassword[i] = getPassword [i + 1];
                    }
                }

                printf("\nPlease enter your age:\n");
                scanf("%s", &age);
                printf("\n");
                strcat(age, "\0");

                strcat(sqltest, username);
                strcat(sqltest, "', '");
                strcat(sqltest, getPassword);
                strcat(sqltest, "', '");
                strcat(sqltest, age);
                strcat(sqltest, "', ");
                strcat(sqltest, "CURRENT_TIMESTAMP);");

                free(input);
                free(getPassword);

                rc = sqlite3_exec(db, sqltest, callback, 0, &zErrMsg);

                if( rc != SQLITE_OK )
                {
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else
                {
                    fprintf(stdout, "User created successfully\n");
                    printf("You are now connected !\n");
                    printf("Welcome %s ^^\n\n", username);
                    connected = 1;
                }

            }else if(choice == 9)
            {
                printf("Goodbye!\n");
                exit(0);
            }
        // Case 2: User is connected
        }else if(connected == 1)
        {
            printf("Now, please choose an option:\n");
            printf("1. Add an other glycemia log\n");
            printf("2. See your glycemia logs\n");
            printf("3. See your glycemia logs for a specific date\n");
            printf("4. Log out\n");
            printf("9. Exit\n");
            scanf("%d", &choice);

            if(choice == 4)
            {
                printf("\nYou are now disconnected\n\n");
                printf("Goodbye %s !\n\n", username);
                connected = 0;
            }else if(choice == 9)
            {
                printf("Goodbye!\n");
                exit(0);
            }
        }
    }while(choice != 9);

    return 0;
}
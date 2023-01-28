//Compilation on linux : gcc testmenu.c sqlite3.c -o testmenu.exe -lpthread -ldl -lm
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>
#include "users/users.h"

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
    char age[3];
    char targeted_glycemia[10];

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

    // Create the table users
    createTableUsers(db, sql, zErrMsg, rc);

    // Print the table users for debug only
    printTableUsers(db, sql, zErrMsg, rc);

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
                scanf("%s", &password);
                strcat(password, "\0");
                printf("\n");

                // Check if the user exists and if the password is correct then connect the user
                loginUser(db, zErrMsg, rc, username, password, connected);

            }else if(choice == 2)
            {

                printf("\nPlease enter your username:\n");
                scanf("%s", &username);
                printf("\n");
                strcat(username, "\0");

                printf("Please enter your age:\n");
                scanf("%s", &age);
                printf("\n");
                strcat(age, "\0");

                printf("Please enter your password:\n");
                scanf("%s", &password);
                printf("\n");
                strcat(password, "\0");

                printf("Please enter your targeted glycemia:\n");
                scanf("%s", &targeted_glycemia);
                printf("\n");
                strcat(targeted_glycemia, "\0");

                // Create a new user
                createUser(db, zErrMsg, rc, username, password, age, targeted_glycemia, connected);

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
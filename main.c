//Compilation on linux : gcc main.c sqlite3.c -o main.exe -lpthread -ldl -lm
//To compile : gcc main.c sqlite3.c database/database.c entries/entry.c glycemia/glycemia.c users/users.c -o main.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sqlite3.h"
#include <time.h>
#include "users/users.h"
#include "database/database.h"
#include "entries/entry.h"
#include "glycemia/glycemia.h"

int main(int argc, char **argv)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    char choice;
    int connected = 0;
    char username[30];
    char password[30];
    char age[3];
    char targeted_glycemia[10];
    int user_id = 3;
    int emptyLogs = 0;

    //CREATE DATABASE;
    rc = sqlite3_open("database/diabetic.db", &db);

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

    do 
    {
        // Case 1: User is not connected
        if(connected == 0)
        {
            printf("Welcome to your glycemia database, please choose an option:\n");
            printf("1. Log in with your user\n");
            printf("2. Create a new user\n");
            printf("9. Exit\n");
            scanf(" %c", &choice);

            if(choice == '1')
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
                loginUser(db, zErrMsg, rc, username, password, &connected);

            }else if(choice == '2')
            {

                printf("\nPlease enter your username:\n");
                scanf("%s", &username);
                printf("\n");
                strcat(username, "\0");

                printf("Please enter your age:\n");
                scanf("%s", &age);
                printf("\n");
                strcat(age, "\0");

                printf("Please enter your password (Need 1 maj [A...Z], 1 min [a...z] and 1 number [0...9]):\n");
                scanf("%s", &password);
                printf("\n");
                strcat(password, "\0");

                printf("Please enter your targeted glycemia:\n");
                scanf("%s", &targeted_glycemia);
                printf("\n");
                strcat(targeted_glycemia, "\0");

                // Create a new user
                createUser(db, zErrMsg, rc, username, password, age, targeted_glycemia, connected);

            }else if(choice == '9')
            {
                printf("Goodbye!\n");
                exit(0);
            }
        // Case 2: User is connected
        }else if(connected == 1)
        {   
            //WE NEED TO GET THE USER_ID GLOBAL ONCE USER IS CONNECTED.
            createTableGlycemia(db,sql,zErrMsg, rc);
            Entry * glycemia = getGlycemiaDataFromDB(user_id);
            Entry *n ; 
;

            if (glycemia == NULL){
                emptyLogs = 1;
            }

            printf("----------MENU-----------:\n");
            printf("1. Add a glycemia log\n");
            printf("2. See your glycemia logs\n");
            printf("3. See your glycemia logs for a specific date\n");
            printf("7. Settings\n");
            printf("8. Log out\n");
            printf("9. Exit\n");
            scanf(" %c", &choice);

            if (choice == '1')
            {
               //if pas de glycémia dans la bdd, createEntry first 
               if (emptyLogs == 1)
               {
                glycemia = createEntry(inputsGlycemia(), "comment", NULL, 1, user_id);
                n = malloc(sizeof(Entry)); 
                n = glycemia; //besoin de malloc ou pas?
               } 
                else 
               {
                n = addEntry(glycemia, inputsGlycemia(), "Newcomment", NULL, 0, user_id);
               }

               //si tout va bien on envoie à la bdd la dernière glycémie
               sendEntryToDatabase(n);
            }else if (choice == '2'){
                //show glycemia logs
                showEntries(glycemia);
                
            }else if(choice == '4')
            {
                printf("\nPlease enter your targeted glycemia:\n");
                scanf("%s", &targeted_glycemia);
                // Update the targeted glycemia
                updateTargetedGlycemia(db, zErrMsg, rc, username, targeted_glycemia);

            }else if(choice == '7')
            {
                connected = 3;   

            }else if(choice == '8')
            {
                printf("\nYou are now disconnected\n\n");
                printf("Goodbye %s !\n\n", username);
                connected = 0;

            }else if(choice == '9')
            {   
                 //Freeing everything before exiting
                while(glycemia){
                  Entry * tmp = glycemia->next;
                  free(glycemia->comment);
                  free(glycemia);
                  glycemia = tmp;
               }
                printf("Goodbye!\n");
                sqlite3_close(db);
                exit(0);
            }
             while(glycemia){
                  Entry * tmp = glycemia->next;
                  free(glycemia->comment);
                  free(glycemia);
                  glycemia = tmp;
                }
        }else if (connected == 2)
        {
            printf("\n----------MENU ADMIN-----------:\n");
            printf("1. Print the table users (debug)\n");
            printf("2. See all  glycemia logs (debug)\n");
            printf("8. Log out\n");
            printf("9. Exit\n");
            scanf(" %c", &choice);

            if(choice == '1')
            {
                // Print the table users for debug only
                printf("\n");
                printTableUsers(db, sql, zErrMsg, rc);

            }else if(choice == '2')
            {
                // Print the table glycemia for debug only
                printf("\n");
                printTableGlycemia(db, sql, zErrMsg, rc);

            }else if(choice == '8')
            {
                printf("\nYou are now disconnected\n\n");
                printf("Goodbye %s !\n\n", username);
                connected = 0;

            }else if(choice == '9')
            {
                printf("Goodbye!\n");
                exit(0);

            }
        }else if (connected == 3)
        {
            printf("\n---------- Settings -----------:\n");
            printf("1. Update your target range glycemia\n");
            printf("2. Change the language\n");
            printf("8. Delete your account\n");
            printf("0. Exit Settings\n");
            scanf(" %c", &choice);

            if(choice == '1')
            {
                printf("\nPlease enter your targeted glycemia:\n");
                scanf("%s", &targeted_glycemia);
                // Update the targeted glycemia
                updateTargetedGlycemia(db, zErrMsg, rc, username, targeted_glycemia);

            }else if(choice == '2')
            {
                printf("\nTO DO\n");
                // printf("\nPlease enter your language:\n");
                // scanf("%s", &language);
                // // Update the targeted glycemia
                // updateLanguage(db, zErrMsg, rc, username, language);
            }else if(choice == '8')
            {
                printf("\nAre you sure you want to delete your account? (y/n)\n");
                scanf(" %c", &choice);
                if(choice == 'y')
                {
                    deleteUser(db, zErrMsg, rc, username, password, connected);
                    printf("\nYou are now disconnected\n\n");
                    printf("Goodbye %s !\n\n", username);
                    connected = 0;
                }else if(choice == 'n')
                {
                    printf("\n");
                }
            }else if(choice == '0')
            {
                connected = 1;
            }
        }
    }while(choice != '9');

    return 0;
}
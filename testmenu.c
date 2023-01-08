#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
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
            printf("You are now connected!\n");
            connected = 1;
        }else if(choice == 2) {
            printf("Please enter your username:\n");
            scanf("%s", &username);
            printf("Please enter your password:\n");
            scanf("%s", &password);
            printf("Please enter your age:\n");
            scanf("%d", &age);
            printf("You are now connected!\n");
            connected = 1;
        }else if(choice == 3) {
            printf("Goodbye!\n");
            exit(0);
        }
    }else if(connected == 1) {
        printf("Now, please choose an option:\n");
        printf("1. Add an other glycemia log\n");
        printf("2. See your glycemia logs\n");
        printf("3. See your glycemia logs for a specific date\n");
        printf("4. Log out\n");
        scanf("%d", &choice);
    }





    return 0;
}
// Code en cours de construction
// This file 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "users/users.h"
#include "database/database.h"
#include "entries/entry.h"
#include "glycemia/glycemia.h"
//#include "updateProfile.h"

// Check if the user is connected
char *menuText = "Please choose an option :\n1 - Update username\n2 - Update password"//\n3 - Update target glycemia\n4 - Delete profile";
char *irreversible = "This action is irreversible. Do you want to continue? (enter 'yes')";
char *enterNewPassword = "Enter your new password :\n";
char *enterNewPasswordAgain = "Enter your new password again :\n";

int choice = -1;

    while (choice!=0)
    {
        printf("%s\n :",menu);
        switch (menu)
        {
        case 1:
            updateUsername(username)
            break;
        case 2:
            updatePassword(username)
            break;/*
        case 3:
            updateTargetGlycemia(username)
            break;
        case 4:
            deleteProfile(username)
            break;
        */
        default:
            break;
        }
    }
    

void updateTable(char *sql) {
    int rc = sqlite3_open("test.db", &db);
    char *zErrMsg = 0;

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
        } else {
            fprintf(stderr, "Opened database successfully\n");
    }

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc!= SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table updated successfully\n");
    }
    sqlite3_close(db);
}

void updatePassword(char *username) {
    char * newPassword = malloc(sizeof(char*));
    char * repeatPassword = malloc(sizeof(char*));
    printf("%s",enterNewPassword);
    scanf("%s",newPassword);
    printf("Enter your new password again :\n");
    scanf("%s",repeatPassword);
    if (strcmp(newPassword,repeatPassword)!=0) {
        printf("Password do not match.");
    } else if (!checkPassword(newPassword)) {
        printf("Password must contain at least 8 characters, 1 number and 1 uppercase");
    } else {
        // Req entière : char sqlPwd[] = "UPDATE USER SET User.password = password WHERE User.username = username";
        char sqlPwd[] = "UPDATE USER SET User.password = ";
        strcat(sqlPwd,newPassword);
        strcat(sqlPwd," WHERE User.username = ");
        strcat(sqlPwd,username);
        strcat(sqlPwd,";");
        updateTable(sqlPwd);
    }
    free(newPassword);
    free(repeatPassword);
}

void updateUsername(char *username) {
    char * newUsername = malloc(sizeof(char*));
    printf("Enter your new username :\n");
    scanf("%s",newUsername);
    if (strlen(newUsername)<2) {
        printf("Your username must be at least 2 characters.");
    } else {
        // Req entière : char sqlUname[] = "UPDATE USER SET User.username = username WHERE User.username = username";
        char sqlName[] = "UPDATE USER SET User.username = ";
        strcat(sqlPwd,newUsername);
        strcat(sqlPwd," WHERE User.username = ");
        strcat(sqlPwd,username);
        strcat(sqlPwd,";");
        updateTable(sqlName);
    }
    free(newUsername);
}

void deleteProfile(char *username) {
    //char * enteredPassword = malloc(sizeof(char*));
    char * confirm = malloc(sizeof(char*));
    printf("%s",irreversible);
    scanf("%s",confirm);
    if(strcmp(confirm,"Yes")==0){
        char sqlDelete[] = "DELETE FROM glycemia WHERE username = ";
        strcat(sqlDelete,username);
        strcat(sqlDelete,";");
        updateTable(sqlDelete);
        char sqlDeleteUser[] = "DELETE FROM Users WHERE username = ";
        strcat(sqlDeleteUser,username);
        strcat(sqlDeleteUser,";");
        updateTable(sqlDeleteUser);
    }
    free(confirm);
}

free(menuText);
free(irreversible);
free(enterNewPassword);
free(enterNewPasswordAgain);


/*
printf("Please enter 1 to switch to a new account or your name is you already have an account :\n");
scanf("%s",name);
if (strcmp(name,49) == 0) {
    // Fonction de connexion
} else {
    printf("Please enter your password :\n");
    scanf("%s",pwd);
}

// check if the password matches with the username
char sqlPasswordCheck[] = "SELECT password FROM User WHERE username = username";
*/
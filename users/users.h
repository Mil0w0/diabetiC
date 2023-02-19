#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../sqlite3.h"
#include "../database/database.h"
#include <time.h>
#include <ctype.h>

void printok();

int checkUser(void *NotUsed, int argc, char **argv, char **azColName);

bool LogIn(char *username, char *password, sqlite3 *db);

void createAdminUser(sqlite3 *db, char *sql, char *zErrMsg, int rc);

int getUserID(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password);

void loginUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int *connected, int *user_id);

void createUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, char *age, char *targeted_glycemia, int connected);

void updateTargetedGlycemia(sqlite3 *db, char *zErrMsg, int rc, char *username, char *targeted_glycemia);

void deleteUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int connected);

bool checkValid(char *username, char *password, char* age, char* targeted_glycemia, sqlite3 *db, char *zErrMsg, int rc);

bool checkPassword(char *password);

bool checkUsername(char *username, sqlite3 *db, char *zErrMsg, int rc);

bool checkGlycemia(char *glycemia);

bool checkAge(char *age);

void cryptPassword(char *password);

void decryptPassword(char *password);
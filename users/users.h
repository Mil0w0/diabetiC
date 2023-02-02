#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../sqlite3.h"
#include <time.h>
#include <ctype.h>

void printok();

int callback(void *NotUsed, int argc, char **argv, char **azColName);

int checkUser(void *NotUsed, int argc, char **argv, char **azColName);

bool LogIn(char *username, char *password, sqlite3 *db);

void createTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void createDatabase(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void printTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void loginUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int *connected);

void createUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, char *age, char *targeted_glycemia, int connected);

void deleteUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int connected);

bool checkValid(char *username, char *password, char* age, char* targeted_glycemia, sqlite3 *db, char *zErrMsg, int rc);

bool checkPassword(char *password);

bool checkUsername(char *username, sqlite3 *db, char *zErrMsg, int rc);

bool checkGlycemia(char *glycemia);

bool checkAge(char *age);
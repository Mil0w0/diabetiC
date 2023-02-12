//
// Created by milo on 06/02/23.
//

int callback(void *NotUsed, int argc, char **argv, char **azColName);

void createTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void createTableGlycemia(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void createDatabase(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void printTableUsers(sqlite3 *db, char *sql, char *zErrMsg, int rc);

void printTableGlycemia(sqlite3 *db, char *sql, char *zErrMsg, int rc);

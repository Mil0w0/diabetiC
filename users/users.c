#include "users.h"

void printok()
{
    printf("OK");
}

int checkUser(void *NotUsed, int argc, char **argv, char **azColName)
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

// CREATE A USER TABLE IF NOT EXISTS;
void createAdminUser(sqlite3 *db, char *sql, char *zErrMsg, int rc)
{

   char username[] = "admin";
   int age = 99;
   bool isAdminCreated = true;
   char password[] = "Respons11";
   float targeted_glycemia = 5.5;
   sqlite3_stmt *res;

   cryptPassword(password);

   isAdminCreated = checkUsername(username, db, zErrMsg, rc);

   if (!isAdminCreated)
   {
    return;
   }

   sql = "INSERT INTO USERS (USERNAME, PASSWORD, AGE, TARGETED_GLYCEMIA, CREATED_AT) VALUES (:username, :password, :age, :targeted_glycemia, CURRENT_TIMESTAMP)";
   /* Execute SQL statement */
   rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

   if (rc == SQLITE_OK) {
      int parameter = sqlite3_bind_parameter_index(res, ":username");
      sqlite3_bind_text(res, parameter, username, strlen(username), NULL);

      parameter =  sqlite3_bind_parameter_index(res, ":password");
      sqlite3_bind_text(res, parameter, password, strlen(password), NULL);

      parameter =  sqlite3_bind_parameter_index(res, ":age");
      sqlite3_bind_int(res, parameter, age);

      parameter =  sqlite3_bind_parameter_index(res, ":targeted_glycemia");
      sqlite3_bind_double(res, parameter, targeted_glycemia);

    } else {
      //Error handling
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    rc = sqlite3_step(res);

    //Error hzndling
    if (rc != SQLITE_DONE) {
        printf("execution failed: %s", sqlite3_errmsg(db));
    }

    //Finishes the request, returns SQLITE_OK if all good if we don't do it can lead to segfaults
    sqlite3_finalize(res);
}

int getUserID(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password)
{
    int user_id;
    sqlite3_stmt *res;

    char *sql = "SELECT ID FROM USERS WHERE username = :username AND password = :password";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc == SQLITE_OK) {
      int parameter = sqlite3_bind_parameter_index(res, ":username");
      sqlite3_bind_text(res, parameter, username, strlen(username), NULL);
      parameter =  sqlite3_bind_parameter_index(res, ":password");
      sqlite3_bind_text(res, parameter, password, strlen(password), NULL);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    while (rc = sqlite3_step(res) == SQLITE_ROW) 
    {
        user_id = sqlite3_column_int(res, 0);
    }

    sqlite3_finalize(res);

    printf("ID : %d\n", rc);

    if( rc == SQLITE_OK )
    {
        printf("Failed to get user ID\n\n");
        sqlite3_free(zErrMsg);
    }

    return user_id;
}

void loginUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int *connected, int *id)
{

    char admin[30] = "admin";
    char adminPassword[30] = "Respons11";

    cryptPassword(password);
    cryptPassword(adminPassword);

    char sqlReq[200] = "SELECT * FROM USERS WHERE username = '";
    strcat(sqlReq, username);
    strcat(sqlReq, "' AND password = '");
    strcat(sqlReq, password);
    strcat(sqlReq, "'");

    rc = sqlite3_exec(db, sqlReq, checkUser, 0, &zErrMsg);

    if( rc == SQLITE_OK )
    {
        printf("Wrong username or password, please try again\n\n");
        sqlite3_free(zErrMsg);
    } else
    {
        if(strcmp(username, admin) == 0 && strcmp(password, adminPassword) == 0)
        {
            printf("You are now in Dev Mode !\n");
            *connected = 2;
        }else
        {
        
        *id = getUserID(db, zErrMsg, rc, username, password);
        printf("You are now connected !\n");
        printf("Welcome %s ^^\n\n", username);
        *connected = 1;
        }
    }
}

void createUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, char* age, char* targeted_glycemia, int connected)
{
    // Check if all the fields are valid
    bool check = false;

    char admin[30] = "admin\0";
    char adminPassword[30] = "Respons11\0";

    check = checkValid(username, password, age, targeted_glycemia, db, zErrMsg, rc);
    if(check == false)
    {
        return;
    }

    cryptPassword(password);
    cryptPassword(adminPassword);

    char sqltest[200] = "INSERT INTO USERS (USERNAME, PASSWORD, AGE, TARGETED_GLYCEMIA, CREATED_AT) VALUES ('\0";
    strcat(sqltest, username);
    strcat(sqltest, "', '");
    strcat(sqltest, password);
    strcat(sqltest, "', '");
    strcat(sqltest, age);
    strcat(sqltest, "', '");
    strcat(sqltest, targeted_glycemia);
    strcat(sqltest, "', ");
    strcat(sqltest, "CURRENT_TIMESTAMP);");

    rc = sqlite3_exec(db, sqltest, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else
    {
        if(strcmp(username, admin) == 0 && strcmp(password, adminPassword) == 0)
        {
            printf("You are now in Dev Mod, don't do shit i'm watching you\n");
            connected = 2;
        }else
        {
        fprintf(stdout, "User created successfully\n");
        printf("You are now connected !\n");
        printf("Welcome %s ^^\n\n", username);
        connected = 1;
        }
    }
}

void updateTargetedGlycemia(sqlite3 *db, char *zErrMsg, int rc, char *username, char* targeted_glycemia)
{
    char sqltest[200] = "UPDATE USERS SET TARGETED_GLYCEMIA = '";
    strcat(sqltest, targeted_glycemia);
    strcat(sqltest, "' WHERE USERNAME = '");
    strcat(sqltest, username);
    strcat(sqltest, "'");

    rc = sqlite3_exec(db, sqltest, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    } else
    {
        fprintf(stdout, "\nTargeted glycemia updated successfully\n\n");
    }
}

void deleteUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int connected)
{
    char sqltest[200] = "DELETE FROM USERS WHERE USERNAME = '";
    strcat(sqltest, username);
    strcat(sqltest, "' AND PASSWORD = '");
    strcat(sqltest, password);
    strcat(sqltest, "'");

    rc = sqlite3_exec(db, sqltest, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    } else
    {
        fprintf(stdout, "User deleted successfully\n");
        connected = 0;
    }
}

bool checkValid(char *username, char *password, char* age, char* targeted_glycemia, sqlite3 *db, char *zErrMsg, int rc)
{
    bool check = false;

    check = checkPassword(password);
    if(check == false)
    {
        printf("Password must contain at least 8 characters, 1 number and 1 uppercase letter\n\n");
        return false;
    }

    check = checkUsername(username, db, zErrMsg, rc);
    if(check == false)
    {
        printf("Username is already taken !\n\n");
        return false;
    }

    check = checkAge(age);
    if(check == false)
    {
        printf("Age must be numeric !\n\n");
        return false;
    }

    check = checkGlycemia(targeted_glycemia);
    if(check == false)
    {
        printf("Targeted glycemia must be numeric !\n\n");
        return false;
    }

    return true;
}

// Create a function to check if the password have at least 8 characters, 1 number and 1 uppercase letter
bool checkPassword(char *password)
{
    int i;
    int length = strlen(password);
    char validChars[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    int hasNumber = 0;
    int hasUppercase = 0;
    int hasSpecialChar = 0;

    if(length < 8)
    {
        return false;
    }

    for(i = 0; i < length; i++)
    {
        if(isdigit(password[i]))
        {
            hasNumber = 1;
        }
        if(isupper(password[i]))
        {
            hasUppercase = 1;
        }
        if(strchr(validChars, password[i]))
        {
            hasSpecialChar = 1;
        }
    }

    if(hasNumber == 1 && hasUppercase == 1 && hasSpecialChar == 1)
    {
        return true;
    } else
    {
        return false;
    }
}

bool checkUsername(char *username, sqlite3 *db, char *zErrMsg, int rc)
{
    char sqlReq[200] = "SELECT * FROM USERS WHERE username = '";
    strcat(sqlReq, username);
    strcat(sqlReq, "'");

    rc = sqlite3_exec(db, sqlReq, checkUser, 0, &zErrMsg);

    if( rc == SQLITE_OK )
    {
        return true;
    } else
    {
        return false;
    }
}

bool checkGlycemia(char *glycemia)
{
    int i, j, check = 0;
    int length = strlen(glycemia);
    char validChars[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < 11; j++)
        {
            if(glycemia[i] == validChars[j])
            {
                check += 1;
            }
        }
    }
    if(check == length)
    {
        return true;
    }
    return false;
}

bool checkAge(char *age)
{
    int i, j, check = 0;
    int length = strlen(age);
    char validChars[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < 10; j++)
        {
            if(age[i] == validChars[j])
            {
                check += 1;
            }
        }
    }
    if(check == length)
    {
        return true;
    }
    return false;
}

void cryptPassword(char *password)
{
    int i;
    int length = strlen(password);

    for(i = 0; i < length; i++)
    {
        password[i] = password[i] + 8;
    }
}

void decryptPassword(char *password)
{
    int i;
    int length = strlen(password);

    for(i = 0; i < length; i++)
    {
        password[i] = password[i] - 8;
    }
}
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

void loginUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, int *connected)
{

    cryptPassword(password);

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
        printf("You are now connected !\n");
        printf("Welcome %s ^^\n\n", username);
        *connected = 1;
    }
}

void createUser(sqlite3 *db, char *zErrMsg, int rc, char *username, char *password, char* age, char* targeted_glycemia, int connected)
{
    // Check if all the fields are valid
    bool check = false;

    check = checkValid(username, password, age, targeted_glycemia, db, zErrMsg, rc);
    if(check == false)
    {
        return;
    }

    cryptPassword(password);

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
        fprintf(stdout, "User created successfully\n");
        printf("You are now connected !\n");
        printf("Welcome %s ^^\n\n", username);
        connected = 1;
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
    char validChars[12] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ','};

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < 10; j++)
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
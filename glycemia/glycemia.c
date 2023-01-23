// This file handles the manipulation about the glycemias.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glycemia.h"

//text variables to be put in an env file later
char defaultUnit[] = "Default glycemia unit is g/L.\n";
char askForGlycemia[] = "Enter current glycemia : ";


//Create Glycemia table in DB.

rc = sqlite3_open("test.db", &db);

if( rc ) {
fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
return(0);
} else {
fprintf(stderr, "Opened database successfully\n");
}

//CREATE A USER TABLE;
/* Create SQL statement */
sql = "CREATE TABLE GLYCEMIA("  \
      "ID            INT PRIMARY KEY  NOT NULL," \
      "VALUE         VARCHAR(30)      NOT NULL," \
      "TAKEN_AT      DATETIME         NOT NULL," \
      "COMMENT       VARCHAR(255)," \
      "USER_ID );";

/* Execute SQL statement */
rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

if( rc != SQLITE_OK ){
fprintf(stderr, "SQL error: %s\n", zErrMsg);
sqlite3_free(zErrMsg);
} else {
fprintf(stdout, "Table created successfully\n");
}
sqlite3_close(db);

//get glycemia from user
double inputsGlycemia(){
    double glycemia;
    double irrealMax = 100; //but can be different if unit is different.

    do {
        printf(defaultUnit);
        printf(askForGlycemia);
        scanf("%lf", &glycemia);
        printf("\n");
    } while (glycemia < 0 || glycemia > irrealMax);
    //need to write double with a dot not comma. how do we check that ? strchr() if it s a string;

    return glycemia;
}
char * inputComment(){}




char getGlycemiaDataFromDB(unsigned int user_id, unsigned int glycemia_position){
    //SQL QUERY
    //"SELECT GLYCEMIA.id, value, taken_at, comment FROM GLYCEMIA, USER WHERE USER.id ="
    //user_id
    //"AND GLYCEMIA.id = "
    //glycemia_position
}




//test code
// int main(int argc, char **argv){
//     double glycemia = inputsGlycemia();
//     printf("%.2lf", glycemia);
// }
//Compilation on linux : gcc main.c sqlite3.c -o main.exe -lpthread -ldl -lm
//To compile : gcc main.c sqlite3.c database/database.c -o main.exe

#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include "database/database.h"

int main(int argc, char **argv) {
   createDatabase();
}
#include "../entries/entry.h"

double inputsGlycemia(char unit[], int user_id, sqlite3 *db);

void alertGlycemiaOutOfRange(double glycemia, int unit, int user_id, sqlite3 *db);

char * inputComment();

double averageGlycemia(Entry *n);

char getDataFromDB(unsigned int user_id);

int whichUnit(char unit[]);
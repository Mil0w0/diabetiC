#include "../entries/entry.h"

double inputsGlycemia(char *unit);

void alertGlycemiaOutOfRange(double glycemia, char *unit);

char * inputComment();

double averageGlycemia(Entry *n);

char getDataFromDB(unsigned int user_id);
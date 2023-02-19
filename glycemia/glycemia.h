#include "../entries/entry.h"

double inputsGlycemia();

void alertGlycemiaOutOfRange(double glycemia, int unit);

char * inputComment();

double averageGlycemia(Entry *n);

char getDataFromDB(unsigned int user_id);
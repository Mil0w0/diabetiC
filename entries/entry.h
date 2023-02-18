#pragma once

typedef struct Entry Entry;
struct Entry {
    Entry *next;
    double value;       //the glycemia
    char * comment;     //an optional comment about the glycemia
    char * taken_at ;   //date when the glycemia was taken (can't be null)
    int entries;        //number of entries of the journal
    int user_id;
};

Entry *createEntry(double value, char *comment, char *date, int position, int user_id);
Entry *addEntry(Entry *firstEntry, double value, char *comment, char *date, int position, int user_id);
int sendEntryToDatabase(Entry *glycemia);
Entry *getGlycemiaDataFromDB(unsigned int user_id);
void printGlycemiaLog(sqlite3_stmt *res);
void showEntries(Entry *firstEntry);
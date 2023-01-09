typedef struct Entry Entry;
struct Entry {
    Entry *next;
    double value; //the glycemia
    char * comment; // an optional comment about the glycemia
    char * taken_at ; //date when the glycemia was taken (can't be null)
    int entries; //number of entries of the journal
};

Entry *createEntry(double value, char *comment, char *date, int position);
void addEntry(Entry *firstEntry, double i, char *comment, char *date, int position);
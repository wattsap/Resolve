#ifndef SYMBOL_LIST   /* Include guard */
#define SYMBOL_LIST

typedef struct symbolEntry
{

char type;
char name[31];
struct symbolEntry * next;
} symbolEntry;

typedef enum errorMessage
{
    multiDef,
    undef,
    
} errorMessage;

void append(symbolEntry *newEntry, symbolEntry *root);

void removeSymbol(symbolEntry *piped, symbolEntry *root);

void  printList(symbolEntry *head);

char getSymbolType(char name[31], symbolEntry *head);

void updateSymbolType(char name[31], symbolEntry *head, char updateType);

int count(symbolEntry *head);

void removeSymbolByName(char *name, symbolEntry *root);

void test();
#endif

static bool isArchive(char * filename);
static bool isObjectFile(char * filename);
static void handleObjectFile(char * filename);
static void handleArchive(char * filename);
static void displayMessageAndExit(char *message);
static void evaluateLine(char *theLine);
static void readFile(char *filename);
static symbolEntry getNewSymbolEntry();
static void addToDList(symbolEntry* newEntry);
static void addToUList(symbolEntry* newEntry);
#define BUFF_SIZE 80
#define MAX_NAME_SIZE 31
#define TYPE_LOCATION 17

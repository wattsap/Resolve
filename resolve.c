/*
* Name: <put your name here>
* Description: This program performs the resolution task of a linker.
*              It takes as input a list of .o files and archive files
*              and keeps track of undefined and defined symbols.  All
*              symbols in a .o file are added to one of the two lists.
*              Archived object files are handled a little differently.
*              If there is a symbol that is currently undefined, but defined
*              in a object file within an archive, then all symbols in that
*              archived object file are added to one of the two lists;
*              otherwise, no symbols from that archive member are added.
*              Members of an archive are visited repeatedly until there are
*              no changes in the lists of defined and undefined symbols.
*
*              If at some point there is an attempt to add a second strong
*              symbol to the defined symbols list, then an error message is
*              displayed indicating the multiple definition.
*
*              At the end of the process, the program looks to make sure that
*              the main function is defined and if not, an error message is
*              displayed.  Next, the program prints out the entries of the
*              undefined symbols list with an error message for each one.
*/

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "symbolList.h"

#define BUFF_SIZE 80
#define MAX_NAME_SIZE 31

static bool isArchive(char * filename);
static bool isObjectFile(char * filename);
static void handleObjectFile(char * filename);
static void handleArchive(char * filename);
static void displayMessageAndExit(char *message);
static void evaluateLine(char *theLine);
static void addToDList(symbolEntry* newEntry);
static void addToUList(symbolEntry* newEntry);
static void readFile(char *filename);
static symbolEntry getNewSymbolEntry();

symbolEntry *uHead;
symbolEntry *dHead;

int main(int argc, char * argv[])
{
  uHead = NULL;

  int i, istat;
  struct stat stFileInfo;

  if (argc <= 1)
  {
    printf("resolve: no input files\n");
    exit(1);
  }
  for (i = 1; i < argc; i++)
  {
    istat = stat(argv[i], &stFileInfo);
    //if istat is 0 then file exists
    if (istat == 0)
    {
      if (!isObjectFile(argv[i]) && !isArchive(argv[i]))
      {
        printf("%s: file not recognized\n", argv[i]);
      } else {
        if (isArchive(argv[i])) handleArchive(argv[i]);
        if (isObjectFile(argv[i])) handleObjectFile(argv[i]);
      }
    } else {
      printf("%s: file not found\n", argv[i]);
    }
  }
  if(!contains("main",dHead))
  {
    printf(": undefined reference to main\n" );
  }
  if(uHead != NULL)
  {
    symbolEntry* temp = uHead;
    while(temp != NULL)
    {
      printf(": undefined reference to %s\n", temp->name);
      temp = temp->next;
    }
    //printing ascii values to see if dloop contains spaces or new lines
    /*
    int loopvar;
    symbolEntry *dHeadLoop = dHead;
    while(dHeadLoop != NULL) {
      printf("Char: %s\n", dHeadLoop->name);
      for (loopvar = 0; loopvar < strlen(dHeadLoop->name); loopvar++){
        printf(" ascii: %d", dHeadLoop->name[loopvar]);
      }
      printf("\n");
      dHeadLoop = dHeadLoop->next;
    }
    */
  }
}

void handleObjectFile(char * filename)
{
  readFile(filename);
}

void handleArchive(char * filename)
{
  readFile(filename);
}

/*
* function: isArchive
* description: This function takes as input a c-string and returns
*              true if the c-string ends with a .a extension.
* input: filename
* returns: 1 or 0
*/
bool isArchive(char * filename)
{
  int len = strlen(filename);
  if (len < 3)
  return false;
  if (filename[len - 2] != '.')
  return false;
  if (filename[len-1] != 'a')
  return false;
  return true;
}

/*
* function: isObjectFile
* description: This function takes as input a c-string and returns
*              true if the c-string ends with a .o extension.
* input: filename
* returns: 1 or 0
*/
bool isObjectFile(char * filename)
{
  int len = strlen(filename);
  if (len < 3)
  return false;
  if (filename[len - 2] != '.')
  return false;
  if (filename[len-1] != 'o')
  return false;
  return true;
}

/*
* function: evaluateFile
* description: reads through an archive file line by line and evaluates each
*		line using
* input: the name of the archive file.
*/
void readFile(char *filename)
{
  char buffer[BUFF_SIZE];
  FILE *fp;
  char nm[BUFF_SIZE];
  strcpy(nm, "nm ");
  char *theCommand = strcat(nm, filename);
  fp = popen(theCommand, "r");
  if (fp == NULL) displayMessageAndExit("popen failed\n");
  while (fgets(buffer, sizeof(buffer), fp))
  {
    evaluateLine(buffer);
  }
  pclose(fp);
}

/*
* function: parseLine
* description: gets the name and type from a string that gets passed to it.
* input: a line from the symbol table.
*/
void evaluateLine(char *theLine)
{
  int index = 0;
  int spaceNum = 0;
  int typeLocation;

  if(theLine[index] == ' ')
  {
    while(theLine[index] == ' ')
    {
      index++;
    }
    typeLocation = index;
  }
  else
  {
    while(theLine[index] != ' ')
    {
      index++;
    }
    typeLocation = index + 1;
  }
  symbolEntry *newEntry = (symbolEntry*)malloc(sizeof(symbolEntry));
  (*newEntry).type = theLine[typeLocation];
  char name[MAX_NAME_SIZE];
  memcpy(name, &theLine[(typeLocation + 2)], (MAX_NAME_SIZE - 1));
  strncpy((*newEntry).name, name,(strlen(name) - 1));
  (*newEntry).next = NULL;
  if(newEntry->type == 't' || newEntry->type == 'T' || newEntry->type == 'd' ||	newEntry->type == 'D')
  {
    addToDList(newEntry);
    printf("d list so far: \n");
    printList(dHead);
  }
  else if(newEntry->type == 'U')
  {
    addToUList(newEntry);
    printf("u list so far: \n");
    printList(dHead);
  }
}

void addToDList(symbolEntry *newEntry)
{
  if (dHead == NULL)
  {
    dHead = newEntry;
  }
  else if (contains(newEntry->name,dHead))
  {
    printf(": multiple definition of %s\n",newEntry->name);
  }
  else
  {
    if (contains(newEntry->name,uHead))
    {
      removeSymbolByName(newEntry->name,uHead);
    }
    append(newEntry, dHead);
  }
}

void addToUList(symbolEntry *newEntry)
{
  if (uHead == NULL)
  {
    uHead = newEntry;
  }
  else
  {
    append(newEntry, uHead);
  }
}

void displayMessageAndExit(char *message)
{
  printf("%s\n", message);
  exit(0);
}

symbolEntry getNewSymbolEntry()
{
  char newName[31];
  char newType;
  symbolEntry newEntry = {newName, newType, NULL};
  return newEntry;
}

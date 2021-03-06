/*/////////////////////////////////////////////////////////////////////////////////////////////
 * SymbolList.c
 * Ben Crawford and Andrew Watts
 * Version: 0.1
 * Description: basic methods for manilpulations of a linked list of symbol entries from the
 * nm pipe command
 * References:
 * - http://www.cprogramming.com/snippets/source-code/singly-linked-list-insert-remove-add-count
 *      -used to learn basic c linked list setup
 */////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolList.h"
#include "stdbool.h"
 //TODO: remove the comments on this
//used for testing
//TODO: remove this struct from the file

/*
 * these are the two fields taken from the nm pipe to get symbol name and type
 */
char* pulledName;
char pulledSymbol;
/*
 * these are the the two roots for the two linked lists:
 * -undefined symbols
 * -defined symbols
 */
symbolEntry uRoot;
symbolEntry dRoot;
/*
 * void append
 * @description: appends a new node to the end of a list
 * @param newEntry: node item to be entered
 * @param root: head of list newEntry will be added to
 * @modifys: adds an symbolEntry
 * @returns: none
 */
void append(symbolEntry *newEntry, symbolEntry *head)
{
    symbolEntry *pCurrent = head;
    while(pCurrent->next != NULL){
      pCurrent = pCurrent->next;
      }
      pCurrent->next = newEntry;
}
/*
 * woid removeSymbol
 * @description: removes node from the list if name and symbol type match node
 * to be removed values
 * @param piped: node that needs to be removed
 * @param root: head of list
 * @modifys: removes item from list
 * @returns: none
 */
 /*
void removeSymbol(symbolEntry *toBeRemoved, symbolEntry *root) {
    struct symbolEntry *temp, *prev;
    temp = root;
    prev = root;
    while(temp != NULL) {
        if((temp->type == toBeRemoved->type) && (strcmp(temp->name, toBeRemoved->name)== 0)) {
            if(temp == root) {
                    root = temp->next;
                    //temp = NULL;
            } else {
                prev->next = temp->next;
                //temp = NULL;
            }
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
}
*/
void removeSymbol(symbolEntry *toBeRemoved, symbolEntry *root) {
    if(toBeRemoved->type == root->type && strcmp(toBeRemoved->name, root->name) == 0) {
        root = root->next;
    } else {
        struct symbolEntry *temp, *prev;
        temp = root;
        prev = root;
        while (temp != NULL) {
            if (temp->type == toBeRemoved->type && strcmp(temp->name, toBeRemoved->name) == 0) {
                prev->next = prev->next->next;
                break;
            }
                prev = temp;
                temp = temp->next;
        }
    }
}
/*
 * char getSymbolType
 * @description: get the symbol type associated with a symbol name
 * @param name: name of symbol type we are searching
 * @param head: head of the list we are searching in
 * @modifys: none
 * @returns: the type of the symbol we are searching
 */
char getSymbolType(char name[31], symbolEntry *head) {
    struct symbolEntry *root;
    root = head;
    while (root != NULL) {
        if(strcmp(root->name,name) == 0) {
            return root->type;
        } else {
            root = root->next;
        }
    }
    return '?'; // means it did not work
}
/*
 * void updateSymbolType
 * @description: updates the type of a symbol
 * @param name: the name of the symbol to be updated
 * @param head: the head of the list to be searched
 * @param updateType: the new type of the symbol
 * @modifys: changes the symbol type
 * @returns: none
 */
void updateSymbolType(char name[31], symbolEntry *head, char updateType) {
    struct symbolEntry *root;
    root = head;
    while(root != NULL) {
        if (strcmp(root->name, name) == 0) {
            root->type = updateType;
            break;
        } else if (root->next == NULL){
            break;
        } else {
            root = root->next;
        }
    }
}

//import stdbool!!
bool contains(char name[31], symbolEntry *head) {
    struct symbolEntry *root;
    root = head;
    while (root != NULL) {
        if(strcmp(root->name,name) == 0) {
            return true;
        } else {
            root = root->next;
        }
    }
    return false; // means it did not work
}

bool contains2(char *name, symbolEntry *head) {
    struct symbolEntry *root;
    root = head;
    while (root != NULL) {
        if (strcmp(root->name,name) == 0) {
            return true;
        } else { 
            root = root->next;
        }
    }
}


/*
 * void print
 * @description: prints the list
 * @param head: the head of the list to be printed
 * @modifys: none
 * @returns: technically none, but gives everything in the list
 */
void  printList(symbolEntry *head)
{
    if(head == NULL) {
        return;
    }
    while(head != NULL) {
        printf("%c   %s\n", head->type, head->name);
        if(head->next == NULL) {
            break;
        } else {
            head = head->next;
        }
    }
    printf("\n");
}
/*
 * int count
 * @description: counts the number of elements in a list
 * @param head: the beginning of the list
 * @modifys: none
 * @returns: the count
 */
int count(symbolEntry *head) {
    symbolEntry *pCurrent = head;
    int count = 0;
    while( pCurrent != NULL) {
        pCurrent = pCurrent->next;
        count++;
    }
    return count;
}

void test()
{
    printf("test\n");
}

void removeSymbolByName(char *name, symbolEntry *root) {
    struct symbolEntry *temp;
    temp = root;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            removeSymbol(temp, root);
            break;
        }
        temp = temp->next;
    } 
}

bool compareSymbols(symbolEntry *a, symbolEntry *b) {
    return (a->type == b->type && strcmp(a->name, b->name) == 0);
}

/*
int  main()
{
    symbolEntry head, entry1, entry2, entry3;
    
    strcpy(head.name,"func0");
    strcpy(entry1.name, "func1");
    strcpy(entry2.name, "func2");
    strcpy(entry3.name, "func3");
    
    head.type = 'T';
    entry1.type = 'U';
    entry2.type = 'D';
    entry3.type = 't';
    
    head.next = NULL;
    entry1.next = NULL;
    entry2.next = NULL;
    entry3.next = NULL;
    
    symbolEntry *pHead = &head;
    symbolEntry *pEntry1 = &entry1;
    symbolEntry *pEntry2 = &entry2;
    symbolEntry *pEntry3 = &entry3;
    
    //printf("count before add: %d\n", size);
    append(pEntry1, pHead);
    int sizeBeforeContains = count(pHead);
    printf("count before contains: %d\n", sizeBeforeContains);
    if (contains2("func1", pHead)) {
        printf("success\n");
    }
    if (contains("func1", pHead)){
        printf("true\n");
        printf("%d\n", count(pHead));
    }
    if (!contains("hello", pHead)) {
        printf("right\n");
    }
    /*
    if (!contains("hello world", pHead)) {
        printf("right again\n");
    }
    if (contains2("func3", pHead)) {
        printf("true\n");
    }
    
    int size = count(pHead);
    printf("size: %d\n, size");

    
    append(pEntry2, pHead);
    append(pEntry3, pHead);
    printf("test\n");
    size = count(pHead);
    printf("count after add: %d\n", size);

    print(pHead);
    
    char theType = getSymbolType(entry2.name, pHead);
    printf("entry2 type: %c\n", theType);
    
    
    printf("updating symbol type...\n");
    char newType = 'T';
    updateSymbolType(entry2.name, pHead, newType);
    char def = getSymbolType(entry2.name, pHead);
    printf("entry2 type after update: %c\n", def);
    
    print(pHead);
    printf("pEntry2->type: %c\n", pEntry2->type);
    
    
    
    printf("removing symbol foo()...\n");
    removeSymbol(pEntry2, pHead);
    int newCount = count(pHead);
    printf("count after remove: %d\n", newCount);
    
    printf("list after remove: \n");
    print(pHead);

    printf("\n\n");
    char *nombre = head.name;
    bool didContain = contains(nombre, pHead);
    if(didContain){
        printf("contains: %s\n", nombre);
    } else {
        printf("does not contain: %s\n", nombre);
    }
    return 0;
}
*/

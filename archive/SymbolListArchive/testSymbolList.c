#include "symbolList.h"
//#include "SymbolList.c"
#include <stdio.h>
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
        printf("contains test 1 pass\n");
        printf("count after test 1 pass: %d\n", count(pHead));
    } else {
        printf("contains test 1 fail\n");
        printf("count after test 1 fail: %d\n", count(pHead));

    }
    append(pEntry2, pHead);
    if (contains("func2", pHead)){
        printf("contains test 2 pass\n");
        printf("count after test 2 pass: %d\n", count(pHead));
    } else {
        printf("contains test 2 fail\n");
        printf("count after test 2 fail: %d\n", count(pHead));
    }
    if (!contains("hello", pHead)) {
        printf("contains test for non-contained item passed\n");
        printf("count after contains test for non-contained item passed: %d\n", count(pHead));
    } else {
        printf("contains test for non-contained item fail\n");
        printf("count after test for non-contained item fail: %d\n", count(pHead));
    }

    char theType = getSymbolType(entry2.name, pHead);
    if (theType == 'D') {
        printf("Get Symbol Type test 1 passed\n");
    } else {
        printf("Get Symbol Type test 1 failed\n");
    }
    
    char newType = 'T';
    updateSymbolType(entry2.name, pHead, newType);
    if (entry2.type == 'T') {
        printf("Update Symbol Type test 1 pass\n");
    } else {
        printf("update Symbol Type test 1 failed\n");
    }

    char def = getSymbolType(entry2.name, pHead);
    if (def == 'T') {
        printf("Get Symbol Type test 2 passed\n");
    } else {
         printf("Get Symbol Type test 2 failed\n");

    }
    
    printf("print list test: \n");
    printf("-----------------------\n");
    printList(pHead);
    
    printf("Count before remove: %d\n", count(pHead));
    printf("removing symbol foo()...\n");
    removeSymbol(pEntry2, pHead);
    int newCount = count(pHead);
    printf("count after remove: %d\n", newCount);
    
    printf("list after remove: \n");
    printf("-------------------------\n");
    printList(pHead);

    return 0;
}
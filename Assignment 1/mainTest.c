#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing, taken from https://eradman.com/posts/tdd-in-c.html
int tests_run = 0;
#define FAIL() printf("\nFailure in %s() line %d\n", __func__, __LINE__)
#define _assert(test) do { if (!(test)) { FAIL(); return 1; } } while(0)
#define _verify(test) do { int r=test(); tests_run++; if(r) return r; } while(0)

// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void *pItem)
{
    _assert(pItem != NULL);
    complexTestFreeCounter++;
}

// For searching
static bool itemEquals(void *pItem, void *pArg)
{
    return (pItem == pArg);
}

int crud_tests() {
    List *pList = List_create();
    _assert(pList != NULL);
    _assert(List_count(pList) == 0);
    
    // Check empty list
    _assert(List_first(pList) == NULL);
    _assert(List_last(pList) == NULL);
    _assert(List_next(pList) == NULL);
    _assert(List_prev(pList) == NULL);
    _assert(List_curr(pList) == NULL);

    // Add (First node in the list)
    // Current list: first -> 12(curr) <- last
    int numToAdd = 12;
    _assert(List_add(pList, &numToAdd) == 0);
    _assert(List_count(pList) == 1);
    _assert(List_curr(pList) == &numToAdd);
    _assert(List_first(pList) == &numToAdd);
    _assert(List_last(pList) == &numToAdd);

    // Insert (Insert before first node)
    // List: first -> 14(curr) -> 12 <- last
    int numToInsert = 14;
    _assert(List_insert(pList, &numToInsert) == 0);
    _assert(List_count(pList) == 2);
    _assert(List_curr(pList) == &numToInsert);

    // Add (Add in between 2 nodes)
    // List: first -> 14 -> 15(curr) -> 12 <- last
    int numToAddBetween = 15;
    _assert(List_add(pList, &numToAddBetween) == 0);
    _assert(List_count(pList) == 3);
    _assert(List_curr(pList) == &numToAddBetween);

    // Insert (Add in between 2 nodes)
    // List: first -> 14 -> 18(curr) -> 15 -> 12 <- last
    int numToInsertBetween = 18;
    _assert(List_add(pList, &numToInsertBetween) == 0);
    _assert(List_count(pList) == 4);
    _assert(List_curr(pList) == &numToInsertBetween);

    // Prepend
    // List: first -> 16(curr) -> 14 -> 18 -> 15 -> 12 <- last
    int numToPrepend = 16;
    _assert(List_prepend(pList, &numToPrepend) == 0);
    _assert(List_count(pList) == 5);
    _assert(List_curr(pList) == &numToPrepend);

    // Append
    // List: first -> 16 -> 14 -> 18 -> 15 -> 12 <- 17 <- last
    int numToAppend = 17;
    _assert(List_append(pList, &numToAppend) == 0);
    _assert(List_count(pList) == 6);
    _assert(List_curr(pList) == &numToAppend);

    return 0;
}

int all_tests() {
    _verify(crud_tests);
    return 0;
}

int main(int argc, char **argv) {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n");
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
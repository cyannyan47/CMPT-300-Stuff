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
static int howManyNodesFreed = 0;
static void listRemoveFreeFn(void *pItem)
{
    _assert(pItem != NULL);
    howManyNodesFreed++;
}

// For searching
static bool itemEquals(void *pItem, void *pArg)
{
    return (pItem == pArg);
}

int list_crud_tests() {
    List *pList = List_create();
    _assert(pList != NULL);
    _assert(List_count(pList) == 0);
    
    // Check empty list
    _assert(List_first(pList) == NULL);
    _assert(List_last(pList) == NULL);
    _assert(List_next(pList) == NULL);
    _assert(List_prev(pList) == NULL);
    _assert(List_curr(pList) == NULL);
    _assert(List_remove(pList) == NULL);
    _assert(List_trim(pList) == NULL);

    // Add (First node in the list)
    // Current list: first -> 12(curr) <- last
    int numToAdd = 12;
    _assert(List_add(pList, &numToAdd) == 0);
    _assert(List_count(pList) == 1);
    _assert(List_curr(pList) == &numToAdd);
    _assert(List_first(pList) == &numToAdd);
    _assert(List_last(pList) == &numToAdd);

    // Insert (Insert before first node)
    // List: first -> 14(curr) <-> 12 <- last
    int numToInsert = 14;
    _assert(List_insert(pList, &numToInsert) == 0);
    _assert(List_count(pList) == 2);
    _assert(List_curr(pList) == &numToInsert);

    // Add (Add in between 2 nodes)
    // List: first -> 14 <-> 15(curr) <-> 12 <- last
    int numToAddBetween = 15;
    _assert(List_add(pList, &numToAddBetween) == 0);
    _assert(List_count(pList) == 3);
    _assert(List_curr(pList) == &numToAddBetween);

    // Insert (Add in between 2 nodes)
    // List: first -> 14 <-> 18(curr) <-> 15 <-> 12 <- last
    int numToInsertBetween = 18;
    _assert(List_insert(pList, &numToInsertBetween) == 0);
    _assert(List_count(pList) == 4);
    _assert(List_curr(pList) == &numToInsertBetween);

    // Prepend
    // List: first -> 16(curr) <-> 14 <-> 18 <-> 15 <-> 12 <- last
    int numToPrepend = 16;
    _assert(List_prepend(pList, &numToPrepend) == 0);
    _assert(List_count(pList) == 5);
    _assert(List_curr(pList) == &numToPrepend);

    // Append
    // List: first -> 16 <-> 14 <-> 18 <-> 15 <-> 12 <-> 17(curr) <- last
    int numToAppend = 17;
    _assert(List_append(pList, &numToAppend) == 0);
    _assert(List_count(pList) == 6);
    _assert(List_curr(pList) == &numToAppend);

    // Iterate through the list from the start to OOB end
    // List: first -> 16 -> 14 -> 18 -> 15 -> 12 -> 17 -> OOB end (curr)
    _assert(List_first(pList) == &numToPrepend);
    _assert(List_prev(pList) == NULL);
    // From OOB START to first
    _assert(List_next(pList) == &numToPrepend);
    _assert(List_next(pList) == &numToInsert);
    _assert(List_next(pList) == &numToInsertBetween);
    _assert(List_next(pList) == &numToAddBetween);
    _assert(List_next(pList) == &numToAdd);
    _assert(List_next(pList) == &numToAppend);
    // Going OOB end
    _assert(List_next(pList) == NULL);
    _assert(List_next(pList) == NULL);
    // Remove from OOB END
    _assert(List_remove(pList) == NULL);

    // Iterate through the list from the end to OOB start
    // List: OOB START(curr) <- 16 <- 14 <- 18 <- 15 <- 12 <- 17 <- last
    _assert(List_last(pList) == &numToAppend);
    _assert(List_next(pList) == NULL);
    // Froom OOB END to last
    _assert(List_prev(pList) == &numToAppend);
    _assert(List_prev(pList) == &numToAdd);
    _assert(List_prev(pList) == &numToAddBetween);
    _assert(List_prev(pList) == &numToInsertBetween);
    _assert(List_prev(pList) == &numToInsert);
    _assert(List_prev(pList) == &numToPrepend);
    // Going OOB start
    _assert(List_prev(pList) == NULL);
    _assert(List_prev(pList) == NULL);
    // Remove from OOB START
    _assert(List_remove(pList) == NULL);

    // Remove an item at the start of the list
    // Remove 16
    // List: OOB START <- 14(curr) <- 18 <- 15 <- 12 <- 17 <- last
    _assert(List_first(pList) == &numToPrepend);
    _assert(List_remove(pList) == &numToPrepend);
    _assert(List_curr(pList) == &numToInsert);
    _assert(List_first(pList) == &numToInsert);

    // Remove an item at the end of the list
    // Remove 17
    // List: OOB START <- 14<- 18 <- 15 <- 12(curr) <- last
    _assert(List_last(pList) == &numToAppend);
    // Remove a last node with List_remove will make current pointer to OOB END
    _assert(List_remove(pList) == &numToAppend);
    _assert(List_curr(pList) == NULL);
    _assert(List_last(pList) == &numToAdd);

    // Trim the last item
    // Remove 12
    // List: OOB START <- 14<- 18 <- 15(curr) <- last
    _assert(List_trim(pList) == &numToAdd);
    _assert(List_curr(pList) == &numToAddBetween);

    // Free all remaining nodes
    // Remove 3 items left
    howManyNodesFreed = 0;
    List_free(pList, listRemoveFreeFn);
    _assert(howManyNodesFreed == 3);

    return 0;
}



int all_tests() {
    _verify(list_crud_tests);
    return 0;
}

int main(int argc, char **argv) {
    int result = all_tests();
    if (result == 0)
        printf("PASSED\n");
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
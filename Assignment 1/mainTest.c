/**
 * Sample test routine for executing each function at least once.
 * Copyright Brian Fraser, 2020
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition)                                                                          \
    do                                                                                            \
    {                                                                                             \
        if (!(condition))                                                                         \
        {                                                                                         \
            printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
            exit(1);                                                                              \
        }                                                                                         \
    } while (0)

// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void *pItem)
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

// For searching
static bool itemEquals(void *pItem, void *pArg)
{
    return (pItem == pArg);
}

static void testComplex()
{
    // Empty list
    printf("Empty list\n");
    List *pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);

    // Add
    printf("Add 41\n");
    int added = 41;
    CHECK(List_add(pList, &added) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added);

    // Insert
    printf("Insert 42\n");
    int inserted = 42;
    CHECK(List_insert(pList, &inserted) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &inserted);

    // Prepend
    printf("Prepend 43\n");
    int prepended = 43;
    CHECK(List_prepend(pList, &prepended) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &prepended);

    // Append
    printf("Append 44\n");
    int appended = 44;
    CHECK(List_append(pList, &appended) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &appended);

    // Next through it all (from before list)
    printf("Next through it all (from before list)\n");
    CHECK(List_first(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_next(pList) == &prepended);
    CHECK(List_next(pList) == &inserted);
    CHECK(List_next(pList) == &added);
    CHECK(List_next(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_next(pList) == NULL);

    // Prev through it all
    //   starting from past end
    printf("Prev through it all\n");
    CHECK(List_last(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_prev(pList) == &appended);
    CHECK(List_prev(pList) == &added);
    CHECK(List_prev(pList) == &inserted);
    CHECK(List_prev(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_prev(pList) == NULL);

    // Remove first
    printf("Remove first\n");
    CHECK(List_first(pList) == &prepended);
    printf("Pass first\n");
    CHECK(List_remove(pList) == &prepended);
    printf("Pass remove\n");
    CHECK(List_curr(pList) == &inserted);
    printf("Pass curr\n");

    // Trim last
    printf("Trim last\n");
    CHECK(List_trim(pList) == &appended);
    CHECK(List_curr(pList) == &added);

    // Free remaining 2 elements
    printf("Free remaining 2 elements\n");
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    printf("%d\n", complexTestFreeCounter);
    CHECK(complexTestFreeCounter == 2);

    // Concat
    printf("Concat\n");
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    List *pList1 = List_create();
    List_add(pList1, &one);
    List_add(pList1, &two);
    CHECK(List_curr(pList1) == &two);
    List *pList2 = List_create();
    List_add(pList2, &three);
    List_add(pList2, &four);
    CHECK(List_curr(pList1) == &two);

    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 4);
    CHECK(List_first(pList1) == &one);
    CHECK(List_last(pList1) == &four);

    // Search
    printf("Search\n");
    List_first(pList1);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &one) == NULL);
}

int main(int argCount, char *args[])
{
    testComplex();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}
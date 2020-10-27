/*
** List data type
*/

/*----- Includes -----*/
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

/*----- Declarations -----*/
static bool isInitialized = false;
static List listArr[LIST_MAX_NUM_HEADS];
static Node nodeArr[LIST_MAX_NUM_NODES];
static List *freeList = NULL;
static Node *freeNode = NULL;

/*----- Implementations -----*/

// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

/*----- Static functions -----*/

/*
** Initialize listArr and nodeArr
** and link the elements purposely for static memory management
*/
static void Arr_init() {
    /* Initialize list_arr
    ** and link the List elements in listArr together for free List management */
    for (int i = 0 ; i < LIST_MAX_NUM_HEADS ; i++) {
        /* Initialize */
        List newList;
        newList.head = NULL;
        newList.tail = NULL;
        newList.curr = NULL;
        newList.isEmpty = true;
        newList.count = 0;
        newList.state = LIST_NOT_OOB;
        listArr[i] = newList;
        /* Link */
        if (i == 0) {
            freeList = &listArr[i];
        }
        else {
            /* Temporarily use head and tail pointers to link free List elements */
            listArr[i-1].tail = (Node *)&listArr[i];
            listArr[i].head = (Node *)&listArr[i-1];
        }
    }

    /* Initialize node_arr
    ** and link the Node elements in nodeArr together for free Node management */
    for (int i = 0 ; i < LIST_MAX_NUM_NODES ; i++) {
        /* Initialize */
        Node newNode;
        newNode.prev = NULL;
        newNode.next = NULL;
        newNode.item = NULL;
        nodeArr[i] = newNode;
        /* Link */
        if (i == 0) {
            freeNode = &nodeArr[i];
        }
        else {
            nodeArr[i-1].next = &nodeArr[i];
            nodeArr[i].prev = &nodeArr[i-1];
        }
    }
}

/*
** Makes a new, empty node, and returns its reference on success.
** Returns a NULL pointer on failure.
*/
static Node* Node_create() {
    /* Check if there is still free Node from nodeArr
    ** If yes, return the reference to a free Node. Else, return NULL */
    if (freeNode != NULL) {
        Node *ref = freeNode;
        freeNode = freeNode->next;
        if (freeNode != NULL) {
            freeNode->prev = NULL;
        }
        ref->next = NULL;
        return ref;
    }
    else {
        return NULL;
    }
}

/*
** Delete nodeToBeFreed, reset its attributes and link it back to freeNode
** making this node available for future operations.
*/
static void Node_free(Node *nodeToBeFreed) {
    if (nodeToBeFreed == NULL) {
        /* The program should not be executing this case. */
        printf("WARNING: Failed to free a Node. NULL was passed into Node_free().\n");
        return;
    }
    nodeToBeFreed->prev = NULL;
    nodeToBeFreed->next = freeNode;
    nodeToBeFreed->item = NULL;
    if (freeNode != NULL) {
        freeNode->prev = nodeToBeFreed;
    }
    freeNode = nodeToBeFreed;
}

/*
** Delete headToBeFreed, reset its attributes and link it back to freeList
** making this list head available for future operations.
** NOTE: Only to be used after properly handling and freeing the nodes from this List
** to avoid memory leak.
*/
static void Head_free(List *headToBeFreed) {
    if (headToBeFreed == NULL) {
        /* The program should not be executing this case. */
        printf("WARNING: Failed to free a List head. NULL was passed into Head_free().\n");
        return;
    }
    headToBeFreed->head = NULL;
    headToBeFreed->tail = (Node *)freeList;
    if (freeList != NULL) {
        freeList->head = (Node *)headToBeFreed;
    }
    headToBeFreed->curr = NULL;
    headToBeFreed->isEmpty = true;
    headToBeFreed->count = 0;
    headToBeFreed->state = LIST_NOT_OOB;
    freeList = headToBeFreed;
}

/*----- Non-static functions -----*/

// Makes a new, empty list, and returns its reference on success.
// Returns a NULL pointer on failure.
List* List_create() {
    /* First run on List_create() will initalize and set up the listArr and nodeArr */
    if (!isInitialized) {
        Arr_init();
        /* Set isInitialized flag so that the function skip this part in later runs */
        isInitialized = true;
    }

    /* Check if there is still free List from listArr
    ** If yes, return the reference to a free List. Else, return NULL */
    if (freeList != NULL) {
        List *ref = freeList;
        freeList = (List *)(freeList->tail);
        if (freeList != NULL) {
            freeList->head = NULL;
        }
        ref->tail = NULL;
        return ref;
    }
    else {
        return NULL;
    }
}

// Returns the number of items in pList.
int List_count(List* pList) {
    assert(pList != NULL);
    return pList->count;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        pList->curr = NULL;
        return NULL;
    }
    else {
        void *item = pList->head->item;
        pList->curr = pList->head;
        pList->state = LIST_NOT_OOB;
        return item;
    }
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        pList->curr = NULL;
        return NULL;
    }
    else {
        void *item = pList->tail->item;
        pList->curr = pList->tail;
        pList->state = LIST_NOT_OOB;
        return item;
    }
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        return NULL;
    }
    else {
        switch (pList->state) {
            case LIST_OOB_START:
                pList->curr = pList->head;
                pList->state = LIST_NOT_OOB;
                break;
            case LIST_OOB_END:
                pList->curr = NULL;
                break;
            default: /* case LIST_NOT_OOB */
                pList->curr = pList->curr->next;
        }
        /* Check if the operation advances the current item beyond the end */
        if (pList->curr == NULL) {
            pList->state = LIST_OOB_END;
            return NULL;
        }
        else {
            return pList->curr->item;
        }
    }
}

// Backs up pList's current item by one, and returns a pointer to the new current item.
// If this operation backs up the current item beyond the start of the pList, a NULL pointer
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        return NULL;
    }
    else {
        switch (pList->state) {
            case LIST_OOB_START:
                pList->curr = NULL;
                break;
            case LIST_OOB_END:
                pList->curr = pList->tail;
                pList->state = LIST_NOT_OOB;
                break;
            default: /* case LIST_NOT_OOB */
                pList->curr = pList->curr->prev;
        }
        /* Check if the operation backs up the current item beyond the start */
        if (pList->curr == NULL) {
            pList->state = LIST_OOB_START;
            return NULL;
        }
        else {
            return pList->curr->item;
        }
    }
}


// Returns a pointer to the current item in pList.
void* List_curr(List* pList) {
    assert(pList != NULL);
    return pList->curr == NULL ? NULL : pList->curr->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item.
// If the current pointer is before the start of the pList, the item is added at the start. If
// the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem) {
    assert(pList != NULL);
    Node* newNode = Node_create();
    /* Checks if there is still available node */
    if (newNode == NULL) {
        return LIST_FAIL;
    }
    else {
        newNode->item = pItem;
    }

    /* Checks for empty list */
    if (pList->isEmpty) {
        pList->head = newNode;
        pList->tail = newNode;
        pList->curr = newNode;
        pList->isEmpty = false;
    }
    else {
        switch (pList->state) {
            case LIST_OOB_START:
                newNode->next = pList->head;
                pList->head->prev = newNode;
                pList->head = newNode;
                pList->curr = newNode;
                break;
            case LIST_OOB_END:
                newNode->prev = pList->tail;
                pList->tail->next = newNode;
                pList->tail = newNode;
                pList->curr = newNode;
                break;
            default: /* case LIST_NOT_OOB */
                newNode->prev = pList->curr;
                newNode->next = pList->curr->next;
                pList->curr->next = newNode;
                /* Checks if the current item is at the last item */
                if (pList->curr == pList->tail) {
                    pList->curr = newNode;
                    pList->tail = newNode;
                }
                else {
                    newNode->next->prev = newNode;
                    pList->curr = newNode;
                }
        }
    }
    pList->count++;
    pList->state = LIST_NOT_OOB;
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem) {
    assert(pList != NULL);
    Node* newNode = Node_create();
    /* Checks if there is still available node */
    if (newNode == NULL) {
        return LIST_FAIL;
    }
    else {
        newNode->item = pItem;
    }

    /* Checks for empty list */
    if (pList->isEmpty) {
        pList->head = newNode;
        pList->tail = newNode;
        pList->curr = newNode;
        pList->isEmpty = false;
    }
    else {
        switch (pList->state) {
            case LIST_OOB_START:
                newNode->next = pList->head;
                pList->head->prev = newNode;
                pList->head = newNode;
                pList->curr = newNode;
                break;
            case LIST_OOB_END:
                newNode->prev = pList->tail;
                pList->tail->next = newNode;
                pList->tail = newNode;
                pList->curr = newNode;
                break;
            default: /* case LIST_NOT_OOB */
                newNode->next = pList->curr;
                newNode->prev = pList->curr->prev;
                pList->curr->prev = newNode;
                /* Checks if the current item is at the first item */
                if (pList->curr == pList->head) {
                    pList->curr = newNode;
                    pList->head = newNode;
                }
                else {
                    newNode->prev->next = newNode;
                    pList->curr = newNode;
                }
        }
    }
    pList->count++;
    pList->state = LIST_NOT_OOB;
    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem) {
    assert(pList != NULL);
    Node* newNode = Node_create();
    /* Checks if there is still available node */
    if (newNode == NULL) {
        return LIST_FAIL;
    }
    else {
        newNode->item = pItem;
    }

    /* Checks for empty list */
    if (pList->isEmpty) {
        pList->head = newNode;
        pList->tail = newNode;
        pList->curr = newNode;
        pList->isEmpty = false;
    }
    else {
        newNode->prev = pList->tail;
        pList->tail->next = newNode;
        pList->tail = newNode;
        pList->curr = newNode;
    }
    pList->count++;
    pList->state = LIST_NOT_OOB;
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem) {
    assert(pList != NULL);
    Node* newNode = Node_create();
    /* Checks if there is still available node */
    if (newNode == NULL) {
        return LIST_FAIL;
    }
    else {
        newNode->item = pItem;
    }

    /* Checks for empty list */
    if (pList->isEmpty) {
        pList->head = newNode;
        pList->tail = newNode;
        pList->curr = newNode;
        pList->isEmpty = false;
    }
    else {
        newNode->next = pList->head;
        pList->head->prev = newNode;
        pList->head = newNode;
        pList->curr = newNode;
    }
    pList->count++;
    pList->state = LIST_NOT_OOB;
    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList) {
    assert(pList != NULL);
    /* Check if the current pointer is out of bound or the list is empty */
    if (pList->curr == NULL || pList->isEmpty || pList->state == LIST_OOB_START || pList->state == LIST_OOB_END) {
        return NULL;
    }
    else {
        void *item = pList->curr->item;
        Node *nodeToBeFreed = pList->curr;
        if (pList->curr == pList->head && pList->curr == pList->tail) {
            pList->head = NULL;
            pList->tail = NULL;
            pList->curr = NULL;
            pList->isEmpty = true;
        }
        else if (pList->curr == pList->head) {
            pList->curr->next->prev = NULL;
            pList->head = pList->curr->next;
            pList->curr = pList->head;
        }
        else if (pList->curr == pList->tail) {
            pList->curr->prev->next = NULL;
            pList->tail = pList->curr->prev;
            pList->curr = NULL;
            pList->state = LIST_OOB_END;
        }
        else {
            pList->curr->prev->next = pList->curr->next;
            pList->curr->next->prev = pList->curr->prev;
        }
        Node_free(nodeToBeFreed);
        pList->count--;
        if (pList->count == 0) {
            pList->isEmpty = true;
        }
        return item;
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2) {
    assert(pList1 != NULL);
    assert(pList2 != NULL);
    assert(pList1 != pList2);
    if (pList1->isEmpty && pList2->isEmpty) { /* Do nothing */ }
    else if (pList1->isEmpty) {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->isEmpty = false;
        pList1->count = pList2->count;
    }
    else if (pList2->isEmpty) { /* Do nothing */ }
    else {
        pList1->tail->next = pList2->head;
        pList2->head->prev = pList1->tail;
        pList1->tail = pList2->tail;
        pList1->count += pList2->count;
    }
    Head_free(pList2);
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        Head_free(pList);
        return;
    }
    for (Node *curr = pList->head ; curr != NULL ; ) {
        Node* nodeToBeFreed = curr;
        curr = curr->next;
        (*pItemFreeFn)(nodeToBeFreed->item);
        Node_free(nodeToBeFreed);
    }
    Head_free(pList);
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        return NULL;
    }
    else if (pList->head == pList->tail) {
        void *item = pList->tail->item;
        Node *nodeToBeFreed = pList->tail;
        pList->head = NULL;
        pList->tail = NULL;
        pList->curr = NULL;
        pList->isEmpty = true;
        pList->count = 0;
        Node_free(nodeToBeFreed);
        return item;
    }
    else {
        void *item = pList->tail->item;
        Node *nodeToBeFreed = pList->tail;
        pList->tail->prev->next = NULL;
        pList->tail = pList->tail->prev;
        pList->curr = pList->tail;
        pList->count--;
        pList->state = LIST_NOT_OOB;
        Node_free(nodeToBeFreed);
        return item;
    }
}

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
//
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg) {
    assert(pList != NULL);
    if (pList->isEmpty) {
        return NULL;
    }
    else if (pList->curr == NULL && pList->state == LIST_OOB_END) {
        return NULL;
    }
    else {
        if (pList->curr == NULL) {
            pList->curr = pList->head;
        }
        for (Node *curr = pList->curr ; curr != NULL ; curr = curr->next) {
            if ((*pComparator)(curr->item, pComparisonArg)) {
                pList->curr = curr;
                return curr->item;
            }
        }
        pList->curr = NULL;
        pList->state = LIST_OOB_END;
        return NULL;
    }
}

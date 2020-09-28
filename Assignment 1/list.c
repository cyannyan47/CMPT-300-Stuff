#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// Mental note: 
// List array holds all lists upto LIST_MAX_NUM_HEADS
// Node array holds all nodes upto LIST_MAX_NUM_NODES
// A list can take up Nodes as its items

// Need a thing to keep track of the lists and nodes info
// For CRUD stuff
// Idea, have all the nodes doubly linked at the start
// Using the static array itself to link every item together
// And then use a pointer to probe the array
// Access the array through the probe

// For List heads, use a queue to keep track of all the list

// Initialize static (private) node array and list array
// Think of these as storages for nodes and list heads

// Delaring arrays of available List heads and Nodes
static List availableListHeads[LIST_MAX_NUM_HEADS];
static Node availableNodes[LIST_MAX_NUM_NODES];

// Using a queue to keep track of the used list heads
// Queue implementation from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation
// Initialize list queue
// Call once in List_create
// ------------------------------------------------------------------------------------
// dequeue <- front (0) <- 1 <- 2 <- 3 <- 4 <- 5 <- 6 <- 7 <- 8 <- (9) rear <- enqueue
// ------------------------------------------------------------------------------------
// NOTE: This is not an actual queue
// It's some methods that manage some variables to work like a queue
#define QUEUE_SUCCESS 0
#define QUEUE_FAIL -1
static int queueFront, queueRear, queueSize;
unsigned queueCapacity;
static List* availableListHeadsQueue[LIST_MAX_NUM_HEADS];
// List heads queue's methods declaration

static void queue_create();
static int queue_is_full();
static int queue_is_empty();
static int queue_enqueue(List* listHead);
static List* queue_dequeue();
static List* queue_front();
static List* queue_rear();

// For node management
static Node* whereNodeAvailable = NULL;
static void initiate_nodes_doubly_linked_list();

static void initiate_available_list_heads();

// Used in List_create, List_add
// Get an available node
// Will manage the node doubly linked list
static Node* pop_available_node();

static void push_back_to_available(Node* nodeBackToAvailable);

// Flag for checking if List_create has been called before
static bool isFirstListCreate = false;

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
    if (!isFirstListCreate)
    {
        // Initiate doubly linked list of nodes
        initiate_nodes_doubly_linked_list();
        // Initiate list head queue
        initiate_available_list_heads();
    }

    
    List* newListHead = queue_dequeue();
    if (newListHead == NULL) 
    {
        return NULL;
    }

    Node* nodeAddress = pop_available_node();
    
    // Initiate pointers to the first node of the list
    newListHead->pNodeCurrent = nodeAddress;
    newListHead->pNodeFirst = nodeAddress;
    newListHead->pNodeLast = nodeAddress;
    newListHead->count = 1;
    newListHead->listState = LIST_NOT_OOB;

    return newListHead;
}

// Returns the number of items in pList.
int List_count(List* pList)
{
    int itemsNum = 0;
    return itemsNum;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) 
{
    void* temp;
    return temp;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
    void* temp;
    return temp;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
    void* temp;
    return temp;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
    void* temp;
    return temp;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
    void* temp;
    return temp;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
    void* temp;
    return temp;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{

}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn)
{

}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    void* temp;
    return temp;
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
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
    void* temp;
    return temp;
}

// ------------------------------------------------------------------------
// Supporting methods implementation
// ------------------------------------------------------------------------

// List management

static void queue_create()
{
    queueFront = queueSize = 0;
    queueCapacity = LIST_MAX_NUM_HEADS;
    queueRear = queueCapacity - 1;
}

static int queue_is_full()
{
    return (queueSize == queueCapacity);
}

static int queue_is_empty()
{
    return (queueSize == 0); 
}

static int queue_enqueue(List* listHead)
{
    if (queue_is_empty())
    {
        return QUEUE_FAIL;
    }

    queueRear = (queueRear + 1 ) % queueCapacity;
    queueSize += 1;
    availableListHeadsQueue[queueRear] = listHead;
    return QUEUE_SUCCESS;
}

static List* queue_dequeue()
{
    if (queue_is_full())
    {
        return NULL;
    }
    List* item = availableListHeadsQueue[queueFront];
    queueFront = (queueFront + 1) % queueCapacity;
    queueSize -= 1;
    return item;
}

static List* queue_front()
{
    if (queue_is_empty())
    {
        return NULL;
    }
    return availableListHeadsQueue[queueFront];
}

static List* queue_rear()
{
    if (queue_is_empty())
    {
        return NULL;
    }
    return availableListHeadsQueue[queueRear];
}

// Node management

static Node* pop_available_node()
{
    // Case: out of available Nodes
    if (whereNodeAvailable == NULL)
    {
        return NULL;
    }

    Node* return_node = whereNodeAvailable;
    // Move whereNodeAvailable to the next node
    whereNodeAvailable = return_node->nextNode;

    // Resetting return_node
    return_node->item = NULL;
    return_node->nextNode = NULL;
    return_node->previousNode = NULL;

    // Case: popping the last node in nodesAvailable
    if (whereNodeAvailable == NULL)
    {
        return return_node;
    }

    // Maintaining the doubly linked list
    whereNodeAvailable->previousNode = NULL;
    
    return return_node;
}

// Make sure I'm not adding nodes not from availableNodes
// Make sure I don't call this before calling initiate_nodes_doubly_linked_list
static void push_back_to_available(Node* nodeBackToAvailable)
{
    // Cleaning the nodeBackToAvailable
    nodeBackToAvailable->item = NULL;
    nodeBackToAvailable->nextNode = NULL;
    nodeBackToAvailable->previousNode = NULL;

    if (whereNodeAvailable == NULL) 
    {
        whereNodeAvailable = nodeBackToAvailable;
        return;
    }

    nodeBackToAvailable->nextNode = whereNodeAvailable;
    whereNodeAvailable->previousNode = nodeBackToAvailable;

}

// Initializers

static void initiate_nodes_doubly_linked_list()
{
    // Initialize all nodes in availableNodes
    for (int i = 0; i < LIST_MAX_NUM_NODES; i++)
    {
        availableNodes[i].nextNode = NULL;
        availableNodes[i].previousNode = NULL;
        availableNodes[i].item = NULL;
    }

    // First case
    availableNodes[0].nextNode = &availableNodes[1];

    // General case
    for (int i = 1; i < LIST_MAX_NUM_NODES - 1; i++)
    {
        availableNodes[i].nextNode = &availableNodes[i+1];
        availableNodes[i].previousNode = &availableNodes[i-1];
    }

    // Last case
    availableNodes[LIST_MAX_NUM_NODES - 1].previousNode = &availableNodes[LIST_MAX_NUM_NODES - 2];


    // Assigning available node to be at availableNode[0]
    whereNodeAvailable = &availableNodes[0];
}

static void initiate_available_list_heads()
{
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++)
    {
        availableListHeads[i].count = 0;
        availableListHeads[i].pNodeCurrent = NULL;
        availableListHeads[i].pNodeFirst = NULL;
        availableListHeads[i].pNodeLast = NULL;
        availableListHeads[i].listState = LIST_NOT_OOB;
    }

    queue_create();
    // Loading every List heads onto available list heads queue
    for(int i = 0; i < LIST_MAX_NUM_HEADS; i++)
    {
        queue_enqueue(&availableListHeads[i]);
    }
}

// For debugging purposes
void Test_playground()
{

}
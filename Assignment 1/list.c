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
// Note: listHead needs to be reset to default node values before calling
// Default node values: NULL for all pointers, 0 for count
static int queue_enqueue(List* listHead);
static List* queue_dequeue();
static List* queue_front();
static List* queue_rear();

// For node management
static Node* pWhereNodeAvailable = NULL;
// Special nodes for logging out of bound states of list
// Are initialized in the first List_create call
// Only used for comparison
// Should never be push back to availabe nodes
static Node NODE_OOB_START = {.item = NULL, .previousNode = NULL, .nextNode = NULL};
static Node NODE_OOB_END = {.item = NULL, .previousNode = NULL, .nextNode = NULL};

static Node* const P_NODE_OOB_START = &NODE_OOB_START;
static Node* const P_NODE_OOB_END = &NODE_OOB_END;

static void initiate_nodes_doubly_linked_list();

static void initiate_available_list_heads();

// Used in List_create, List_add
// Get an available node by taking a node from node available array
// This method will maintain the structure of the doubly linked list after pop
static Node* pop_available_node();

// Used in List_remove
// Return a removed node from a list to the node available doubly linked list
// This method will maintain the structure of the doubly linked list after push 
// Node clean up is done here
static void push_back_to_available(Node* nodeBackToAvailable);

// Flag for checking if List_create has been called before
static bool isFirstListCreate = false;

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
    // One time setup
    if (!isFirstListCreate)
    {
        // Initiate doubly linked list of nodes
        initiate_nodes_doubly_linked_list();
        // Initiate list head queue
        initiate_available_list_heads();

        isFirstListCreate = true;
    }

    
    List* newListHead = queue_dequeue();
    // Case run out of list heads
    if (newListHead == NULL) 
    {
        printf("Out of List heads\n");
        return NULL;
    }
    
    // Initiate pointers to the first node of the list
    newListHead->pNodeCurrent = NULL;
    newListHead->pNodeFirst = NULL;
    newListHead->pNodeLast = NULL;
    newListHead->count = 0;

    return newListHead;
}

// Returns the number of items in pList.
int List_count(List* pList)
{
    return pList->count;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList) 
{
    if (pList->count == 0)
    {
        pList->pNodeCurrent = NULL;
        return NULL;
    }

    pList->pNodeCurrent = pList->pNodeFirst;
    return pList->pNodeFirst->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
    if (pList->count == 0)
    {
        pList->pNodeCurrent = NULL;
        return NULL;
    }
    
    pList->pNodeCurrent = pList->pNodeLast;
    return pList->pNodeLast->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
    // Case empty list
    // Don't do anything
    if (pList->count == 0)
    {
        return NULL;
    }

    // Case where out of bound before the list head
    if (pList->pNodeCurrent == P_NODE_OOB_START)
    {
        pList->pNodeCurrent = pList->pNodeFirst;
        return pList->pNodeFirst->item;
    }

    // Case where the next node of the current node will be out of bound
    if (pList->pNodeCurrent->nextNode == NULL)
    {
        pList->pNodeCurrent = P_NODE_OOB_END;
        return NULL;
    }

    pList->pNodeCurrent = pList->pNodeCurrent->nextNode;
    return pList->pNodeCurrent->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
    // Case empty list
    // Don't do anything
    if (pList->count == 0)
    {
        return NULL;
    }

    // Case where out of bound after the list end
    if (pList->pNodeCurrent == P_NODE_OOB_END)
    {
        pList->pNodeCurrent = pList->pNodeLast;
        return pList->pNodeLast->item;
    }

    // Case where the previous node of the current node will be out of bound
    if (pList->pNodeCurrent->previousNode == NULL)
    {
        pList->pNodeCurrent = P_NODE_OOB_START;
        return NULL;
    }

    pList->pNodeCurrent = pList->pNodeCurrent->previousNode;
    return pList->pNodeCurrent->item;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
    // Case empty list
    // Don't do anything
    if (pList->count == 0)
    {
        return NULL;
    }

    return pList->pNodeCurrent->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{
    // Getting a node from available nodes
    Node* nodeToAdd = pop_available_node();

    // Out of nodes
    if (nodeToAdd == NULL)
    {
        return LIST_FAIL;
    }

    // Configuring nodeToAdd
    // Remember: the default pointers of node are all NULL
    nodeToAdd->item = pItem;

    // Case first node of the list
    if (pList->count == 0)
    {
        // Updating list info
        pList->pNodeFirst = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->pNodeCurrent = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Case: current item is out of bound after the list
    // or adding nodeToAdd to after the last node
    if (pList->pNodeCurrent == P_NODE_OOB_END || pList->pNodeCurrent == pList->pNodeLast)
    {
        // Linking node to add to after the last node
        nodeToAdd->previousNode = pList->pNodeLast;
        pList->pNodeLast->nextNode = nodeToAdd;

        // Updating pList info
        // Make newNode the current item
        pList->pNodeCurrent = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Case: current item is out of bound before the list
    if (pList->pNodeCurrent == P_NODE_OOB_START)
    {
        // Linking node to add to before the first node
        nodeToAdd->nextNode = pList->pNodeFirst;
        pList->pNodeFirst->previousNode = nodeToAdd;

        // Updating pList info
        // Make newNode the current item
        pList->pNodeCurrent = nodeToAdd;
        pList->pNodeFirst = nodeToAdd;
        pList->count += 1;



        return LIST_SUCCESS;
    }

    // Normal case: adding a node after the current item node

    // Linking nodeToAdd in between pCurrentNode and after pCurrentNode
    nodeToAdd->nextNode = pList->pNodeCurrent->nextNode;
    nodeToAdd->previousNode = pList->pNodeCurrent;

    nodeToAdd->nextNode->previousNode = nodeToAdd;
    nodeToAdd->previousNode->nextNode = nodeToAdd;

    // Make newNode the current item
    // Updating pList info
    pList->pNodeCurrent = nodeToAdd;
    pList->count += 1;

    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
    // Getting a node from available nodes
    Node* nodeToAdd = pop_available_node();

    // Out of nodes
    if (nodeToAdd == NULL)
    {
        return LIST_FAIL;
    }

    // Configuring nodeToAdd
    // Remember: the default pointers of node are all NULL
    nodeToAdd->item = pItem;

    // Case first node of the list
    if (pList->count == 0)
    {
        // Updating list info
        pList->pNodeFirst = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->pNodeCurrent = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Case: current item is out of bound after the list
    if (pList->pNodeCurrent == P_NODE_OOB_END)
    {
        // Linking node to add to after the last node
        nodeToAdd->previousNode = pList->pNodeLast;
        pList->pNodeLast->nextNode = nodeToAdd;

        // Updating pList info
        // Make newNode the current item
        pList->pNodeCurrent = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Case: current item is out of bound before the list
    if (pList->pNodeCurrent == P_NODE_OOB_START || pList->pNodeCurrent == pList->pNodeFirst)
    {
        // Linking node to add to before the first node
        nodeToAdd->nextNode = pList->pNodeFirst;
        pList->pNodeFirst->previousNode = nodeToAdd;

        // Updating pList info
        // Make newNode the current item
        pList->pNodeCurrent = nodeToAdd;
        pList->pNodeFirst = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Normal case: adding a node after the current item node

    // Linking nodeToAdd in between pCurrentNode and before pCurrentNode
    nodeToAdd->nextNode = pList->pNodeCurrent;
    nodeToAdd->previousNode = pList->pNodeCurrent->previousNode;

    nodeToAdd->nextNode->previousNode = nodeToAdd;
    nodeToAdd->previousNode->nextNode = nodeToAdd;

    // Make newNode the current item
    // Updating pList info
    pList->pNodeCurrent = nodeToAdd;
    pList->count += 1;

    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
    // Getting a node from available nodes
    Node* nodeToAdd = pop_available_node();

    // Out of nodes
    if (nodeToAdd == NULL)
    {
        return LIST_FAIL;
    }

    // Configuring nodeToAdd
    // Remember: the default pointers of node are all NULL
    nodeToAdd->item = pItem;

    // Case first node of the list
    if (pList->count == 0)
    {
        // Updating list info
        pList->pNodeFirst = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->pNodeCurrent = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Linking node to add to after the last node
    nodeToAdd->previousNode = pList->pNodeLast;
    pList->pNodeLast->nextNode = nodeToAdd;

    // Updating the last node pointer
    pList->pNodeLast = nodeToAdd;
    pList->count += 1;

    // Make newNode the current item
    pList->pNodeCurrent = nodeToAdd;

    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
    // Getting a node from available nodes
    Node* nodeToAdd = pop_available_node();

    // Out of nodes
    if (nodeToAdd == NULL)
    {
        return LIST_FAIL;
    }

    // Configuring nodeToAdd
    // Remember: the default pointers of node are all NULL
    nodeToAdd->item = pItem;

    // Case first node of the list
    if (pList->count == 0)
    {
        // Updating list info
        pList->pNodeFirst = nodeToAdd;
        pList->pNodeLast = nodeToAdd;
        pList->pNodeCurrent = nodeToAdd;
        pList->count += 1;

        return LIST_SUCCESS;
    }

    // Linking node to add to before the first node
    nodeToAdd->nextNode = pList->pNodeFirst;
    pList->pNodeFirst->previousNode = nodeToAdd;

    // Updating the first node pointer
    pList->pNodeFirst = nodeToAdd;
    pList->count += 1;

    // Make newNode the current item
    pList->pNodeCurrent = nodeToAdd;

    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
    // Case: List is already empty
    if (pList->count == 0)
    {
        return NULL;
    }

    // Case: current node is OOB
    if (pList->pNodeCurrent == P_NODE_OOB_END || pList->pNodeCurrent == P_NODE_OOB_START)
    {
        return NULL;
    }

    Node* nodeToRemove = pList->pNodeCurrent;

    // Case: current node is the last node
    if (nodeToRemove == pList->pNodeLast)
    {
        // Detach nodeToRemove from the List
        nodeToRemove->previousNode->nextNode = NULL;
        
        // Update list info
        pList->pNodeCurrent = P_NODE_OOB_END;
        pList->pNodeLast = nodeToRemove->previousNode;
        pList->count -= 1;

        void* nodeToRemoveItem = nodeToRemove->item;
        // Clean up is done in push_
        push_back_to_available(nodeToRemove);

        return nodeToRemoveItem;
    }

    if (nodeToRemove == pList->pNodeFirst)
    {
        // Detach nodeToRemove from the List
        nodeToRemove->nextNode->previousNode = NULL;

        // Update list info
        pList->pNodeCurrent = nodeToRemove->nextNode;
        pList->pNodeFirst = nodeToRemove->nextNode;
        pList->count -= 1;

        void* nodeToRemoveItem = nodeToRemove->item;
        // Clean up is done in push_
        push_back_to_available(nodeToRemove);

        return nodeToRemoveItem;
    }
    // Linking the previous and next node of nodeToRemove
    nodeToRemove->previousNode->nextNode = nodeToRemove->nextNode;
    nodeToRemove->nextNode->previousNode = nodeToRemove->previousNode;

    // Update list info
    pList->pNodeCurrent = nodeToRemove->nextNode;
    pList->count -= 1;

    void* nodeToRemoveItem = nodeToRemove->item;
    push_back_to_available(nodeToRemove);

    return nodeToRemoveItem;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
    // Case: List 2 is empty
    // return pList2 back to the List queue
    // Don't care about List 1
    if (pList2->count == 0)
    {
        // Clean up list2
        pList2->pNodeCurrent = NULL;
        pList2->pNodeFirst = NULL;
        pList2->pNodeLast = NULL;
        // Return list2 back to list queue
        queue_enqueue(pList2);

        return;
    }

    // Case: List 1 is empty
    // Assign pointer to list 2 pointer to list 1
    // Reset List 2 pointer 
    if (pList1->count == 0)
    {
        // Hard copy list 2 stuff to list 1
        pList1->count = pList2->count;
        pList1->pNodeFirst = pList2->pNodeFirst;
        pList1->pNodeCurrent = pList2->pNodeCurrent;
        pList1->pNodeLast = pList2->pNodeLast;

        // Clean list 2
        pList2->count = 0;
        pList2->pNodeFirst = NULL;
        pList2->pNodeCurrent = NULL;
        pList2->pNodeLast = NULL;

        // Return list 2 back to list queue
        queue_enqueue(pList2);

        return;
    }

    // Normal case
    // Both List 1 and List 2 have nodes

    // Link last node of list 1 to first node of list 2
    pList1->pNodeLast->nextNode = pList2->pNodeFirst;
    pList2->pNodeFirst->previousNode = pList1->pNodeLast;

    // Updating List 1 info
    pList1->count = pList1->count + pList2->count;
    pList1->pNodeLast = pList2->pNodeLast;

    // Cleaning List 2 info to return to available list queue
    pList2->count = 0;
    pList2->pNodeFirst = NULL;
    pList2->pNodeCurrent = NULL;
    pList2->pNodeLast = NULL;

    // Return list 2 back to list queue
    queue_enqueue(pList2);

    return;

}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    pList->pNodeCurrent = pList->pNodeLast;
    for (pList->count; pList->count > 0; )
    {
        printf("Count %d\n", pList->count);
        Node* pNodeToFree = pList->pNodeCurrent;
        pList->pNodeCurrent = pList->pNodeCurrent->previousNode;

        // Freeing the item inside the node
        (*pItemFreeFn)(pNodeToFree->item);
        push_back_to_available(pNodeToFree);

        pList->count--;
        printf("After subtract count %d\n", pList->count);
    }
    // while(pList->pNodeCurrent != pList->pNodeFirst) 
    // {
    //     printf("Count %d\n", pList->count);
    //     Node* pNodeToFree = pList->pNodeCurrent;
    //     pList->pNodeCurrent = pList->pNodeCurrent->previousNode;

    //     // Freeing the item inside the node
    //     (*pItemFreeFn)(pNodeToFree->item);
    //     push_back_to_available(pNodeToFree);

    //     pList->count--;
    //     printf("After subtract count %d\n", pList->count);
    // }
    printf("After while loop\n");

    if (pList->count == 1 && pList->pNodeFirst == pList->pNodeLast)
    {
        printf("Count qafkyfafgjg %d\n", pList->count);
        if (pList->pNodeFirst == NULL)
        {
            printf("First node exists\n");
        }
        (*pItemFreeFn)(pList->pNodeFirst->item);
        push_back_to_available(pList->pNodeFirst);
        pList->count = 0;
    }
    
    pList->pNodeFirst = NULL;
    pList->pNodeCurrent = NULL;
    pList->pNodeLast = NULL;

    queue_enqueue(pList);

    return;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    if (pList->count == 0)
    {
        return NULL;
    }

    void* itemToReturn = pList->pNodeLast->item;
    Node* pNodeToFree = pList->pNodeLast;

    pList->pNodeLast = pList->pNodeLast->previousNode;
    pList->pNodeCurrent = pList->pNodeLast;
    pList->count -= 1;

    push_back_to_available(pNodeToFree);
    return itemToReturn;
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
    if (pList->count == 0 || pList->pNodeCurrent == P_NODE_OOB_END)
    {
        return NULL;
    }

    if (pList->pNodeCurrent == P_NODE_OOB_START)
    {
        pList->pNodeCurrent = pList->pNodeFirst;
    } 

    while (pList->pNodeCurrent != pList->pNodeLast) 
    {
        int isMatch = (*pComparator)(pList->pNodeCurrent->item, pComparisonArg);
        if (isMatch == 1)
        {
            return pList->pNodeCurrent->item;
        }

        // Not found, move to the next node
        pList->pNodeCurrent = pList->pNodeCurrent->nextNode;
    }

    // Comparing item at last node
    int isMatch = (*pComparator)(pList->pNodeCurrent->item, pComparisonArg);
    if (isMatch == 1)
    {
        return pList->pNodeCurrent->item;
    }
    
    pList->pNodeCurrent = P_NODE_OOB_END;
    return NULL;
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
    if (queue_is_full())
    {
        return QUEUE_FAIL;
    }

    queueRear = (queueRear + 1) % queueCapacity;
    queueSize += 1;
    availableListHeadsQueue[queueRear] = listHead;
    return QUEUE_SUCCESS;
}

static List* queue_dequeue()
{
    if (queue_is_empty())
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
    if (pWhereNodeAvailable == NULL)
    {
        return NULL;
    }

    Node* return_node = pWhereNodeAvailable;
    // Move whereNodeAvailable to the next node
    pWhereNodeAvailable = return_node->nextNode;

    // Resetting/Cleaning return_node before pop out for list
    return_node->item = NULL;
    return_node->nextNode = NULL;
    return_node->previousNode = NULL;

    // Case: popping the last node in nodesAvailable
    if (pWhereNodeAvailable == NULL)
    {
        return return_node;
    }

    // Maintaining the doubly linked list
    pWhereNodeAvailable->previousNode = NULL;
    
    return return_node;
}

// Make sure I'm not adding nodes not from availableNodes
// Make sure I don't call this before calling initiate_nodes_doubly_linked_list
static void push_back_to_available(Node* nodeBackToAvailable)
{
    if (nodeBackToAvailable == P_NODE_OOB_END || nodeBackToAvailable == P_NODE_OOB_START)
    {
        printf("You're pushing sentinel nodes into the node pool\n");
        printf("Aborting\n");
        return;
    }
    // Cleaning the nodeBackToAvailable
    nodeBackToAvailable->item = NULL;
    nodeBackToAvailable->nextNode = NULL;
    nodeBackToAvailable->previousNode = NULL;

    if (pWhereNodeAvailable == NULL) 
    {
        pWhereNodeAvailable = nodeBackToAvailable;
        return;
    }

    // Maintain the doubly linked list
    nodeBackToAvailable->nextNode = pWhereNodeAvailable;
    pWhereNodeAvailable->previousNode = nodeBackToAvailable;

    // Point to the newest available node
    pWhereNodeAvailable = nodeBackToAvailable;

}

// Initializers

// Initiate all nodes with default values
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
    pWhereNodeAvailable = &availableNodes[0];
}

// Initiate all list heads with default values
// Default values: NULL for all pointers, 0 for count
static void initiate_available_list_heads()
{
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++)
    {
        availableListHeads[i].count = 0;
        availableListHeads[i].pNodeCurrent = NULL;
        availableListHeads[i].pNodeFirst = NULL;
        availableListHeads[i].pNodeLast = NULL;
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
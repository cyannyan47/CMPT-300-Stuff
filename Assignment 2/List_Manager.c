#include <pthread.h>

#include "list.h"
#include "List_Manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static List* transmitter_list = NULL;
static List* receiver_list = NULL;
static pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t transmitter_list_avail = PTHREAD_COND_INITIALIZER;
static pthread_cond_t receiver_list_avail = PTHREAD_COND_INITIALIZER;

void List_Manager_init() {
    pthread_mutex_lock(&list_mutex);
    {
        transmitter_list = List_create();
        receiver_list = List_create();
    }
    pthread_mutex_unlock(&list_mutex);
}

void Transmitter_List_prepend(char *s_pMsgAllocated) {
    pthread_mutex_lock(&list_mutex);
    {
        printf("Kb_in: Prepending message to transmitter list.\n");
        if (List_prepend(transmitter_list, (void *)s_pMsgAllocated) == LIST_FAIL) {
            printf("Kb_in: Prepending message to transmitter list FAILED.\n");
        }
        pthread_cond_signal(&transmitter_list_avail);
    }
    pthread_mutex_unlock(&list_mutex);
}

void Transmitter_List_trim(char **s_pMsgAllocated) {
    void *item;
    pthread_mutex_lock(&list_mutex);
    {
        if (transmitter_list->isEmpty) {
            pthread_cond_wait(&transmitter_list_avail, &list_mutex);
        }
        printf("UDP_Tx: Trimming message from transmitter list.\n");
        item = List_trim(transmitter_list);
    }
    pthread_mutex_unlock(&list_mutex);
    
    if (item == NULL) {
        printf("UDP_Tx: Trimming message from transmitter list FAILED.\n");
    }
    else {
        printf("Before strcpy\n");
        // strcpy(s_pMsgAllocated, (const char *)item);
        *s_pMsgAllocated = (char *)item;
        printf("After strcpy\n");
    }
}

void Receiver_List_prepend(char *s_pMsgAllocated) {
    pthread_mutex_lock(&list_mutex);
    {
        printf("UDP_Rx: Prepending message to receiver list.\n");
        if (List_prepend(receiver_list, (void *)s_pMsgAllocated) == LIST_FAIL) {
            printf("UDP_Rx: Prepending message to receiver list FAILED.\n");
        }
        pthread_cond_signal(&receiver_list_avail);
    }
    pthread_mutex_unlock(&list_mutex);
}

void Receiver_List_trim(char **s_pMsgAllocated) {
    void *item;
    pthread_mutex_lock(&list_mutex);
    {
        if (transmitter_list->isEmpty) {
            pthread_cond_wait(&receiver_list_avail, &list_mutex);
        }
        printf("Scr_out: Trimming message from receiver list.\n");
        item = List_trim(receiver_list);
    }
    pthread_mutex_unlock(&list_mutex);

    if (item == NULL) {
        printf("Scr_out: Trimming message from receiver list FAILED.\n");
    }
    else {
        // strcpy(s_pMsgAllocated, (const char *)item);
        *s_pMsgAllocated = (char *)item;
    }
}

void freeFN(void *ptr) {
    free(ptr);
    ptr = NULL;
}

void List_Manager_shutdown() {
    pthread_mutex_lock(&list_mutex);
    {
        List_free(transmitter_list, freeFN);
        List_free(receiver_list, freeFN);
    }
    pthread_mutex_unlock(&list_mutex);
}
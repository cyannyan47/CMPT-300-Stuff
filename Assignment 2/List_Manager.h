#ifndef LIST_MANAGER_H
#define LIST_MANAGER_H

#include "list.h"
void List_Manager_init();
void List_Manager_shutdown();

void Transmitter_List_prepend(char *msg);
void Transmitter_List_trim(char **msg);
void Transmitter_List_signal();
void Receiver_List_prepend(char *msg);
void Receiver_List_trim(char **msg);
void Receiver_List_signal();

void Transmitter_List_Non_Empty_Signal();
void Receiver_List_Non_Empty_Signal();

#endif
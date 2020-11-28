

#ifndef CLIMANAGER_H
#define CLIMANAGER_H

#include <stdbool.h>

#define MAX_MSG_LENGTH 40

void CLI_Init();

// Check if there's any process running 
bool CLI_IsInitExited();

void CLI_Create(int prio);

void CLI_Fork();

void CLI_Kill(int pid);

void CLI_Exit();

void CLI_Quantum();

void CLI_Send(int pid, char *msg);

void CLI_Receive();

void CLI_Reply(int pid, char *msg);

void CLI_SemNew(int semID, int initValue);

void CLI_SemP(int semID);

void CLI_SemV(int semID);

void CLI_ProcInfo(int pid);

void CLI_TotalInfo();

void CLI_End();

#endif
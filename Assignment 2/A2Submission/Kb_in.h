#ifndef KB_IN_H
#define KB_IN_H

#include <pthread.h>
#include "list.h"

#define MAX_LENGTH 1024

#define KB_IN_FAIL -1
#define KB_IN_SUCCESS 0

void Kb_in_init();

void Kb_in_Shutdown();

#endif
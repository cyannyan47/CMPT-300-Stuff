#include<stdio.h>
#include<stdlib.h>
#include "CLIManager.h"
int main() {

    // Simulation starting
    // Init CLIManager
    CLI_Init();

    while (!CLI_IsInitExited()) {
        // Accept CLI in here

    }


    return 0;
}
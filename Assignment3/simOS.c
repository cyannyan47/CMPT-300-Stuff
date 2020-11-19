#include<stdio.h>
#include<stdlib.h>
#include "CLIManager.h"
int main() {

    // Simulation starting
    // Init CLIManager
    CLI_Init();

    while (!CLI_IsInitExited()) {
        // Accept CLI in here
        printf("Enter your command:\n");
        char command;
        scanf("%c", &command);
        switch(command) {
            case 'C':
                printf("Create: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'F':
                printf("Fork: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'K':
                printf("Kill: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'E':
                printf("Exit: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'Q':
                printf("Quantum: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'S':
                printf("Send: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'R':
                printf("Receive: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'Y':
                printf("Reply: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'N':
                printf("New Semaphore: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'P':
                printf("Semaphore P: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'V':
                printf("Semaphore V: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'I':
                printf("Procinfo: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            case 'T':
                printf("Totalinfo: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                break;
            default:
                printf("Invalid command. Try again\n");
                break;
        }

    }

    CLI_End();
    return 0;
}
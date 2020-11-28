#include<stdio.h>
#include<stdlib.h>
#include "CLIManager.h"
int main() {

    // Simulation starting
    // Init CLIManager
    CLI_Init();

    while (!CLI_IsInitExited()) {
        // Accept CLI in here
        int numInput, numInput2;
        char msg[40];
        printf("Enter your command: ");
        char command;
        scanf("%c", &command);
        getchar();
        switch(command) {
            case 'C':
                printf("Create: Enter process' priority (0.High; 1.Norm; 2.Low)\n");
                scanf("%d", &numInput);
                getchar();
                CLI_Create(numInput);
                break;
            case 'F':
                printf("Fork: Running Fork\n");
                CLI_Fork();
                break;
            case 'K':
                printf("Kill: Enter process' pid\n");
                scanf("%d", &numInput);
                getchar();
                CLI_Kill(numInput);
                break;
            case 'E':
                printf("Exit: Exiting current process\n");
                CLI_Exit();
                break;
            case 'Q':
                printf("Quantum: Signalling next quantum time\n");
                CLI_Quantum();
                break;
            case 'S':
                printf("Send: Enter process' pid to send\n");
                scanf("%d", &numInput);
                getchar();
                printf("Send: Enter send message\n");
                fgets(msg, 40, stdin);
                CLI_Send(numInput, msg);
                break;
            case 'R':
                printf("Receive: Making the running process wait for a send message\n");
                CLI_Receive();
                break;
            case 'Y':
                printf("Reply: Enter process' pid to reply\n");
                scanf("%d", &numInput);
                getchar();
                printf("Reply: Enter reply message\n");
                fgets(msg, 40, stdin);
                CLI_Reply(numInput, msg);
                break;
            case 'N':
                printf("New Semaphore: Enter semaphore ID (From 0 to 4)\n");
                scanf("%d", &numInput);
                getchar();
                printf("New Semaphore: Enter semaphore initial value\n");
                scanf("%d", &numInput2);
                getchar();
                CLI_SemNew(numInput, numInput2);
                break;
            case 'P':
                printf("Semaphore P: Enter initialized semaphore ID\n");
                scanf("%d", &numInput);
                getchar();
                CLI_SemP(numInput);
                break;
            case 'V':
                printf("Semaphore P: Enter initialized semaphore ID\n");
                scanf("%d", &numInput);
                getchar();
                CLI_SemV(numInput);
                break;
            case 'I':
                printf("Procinfo: Enter process' pid to dump complete state\n");
                scanf("%d", &numInput);
                getchar();
                CLI_ProcInfo(numInput);
                break;
            case 'T':
                printf("Totalinfo: Displaying all process queues and their contents\n");
                CLI_TotalInfo();
                break;
            default:
                printf("Invalid command. Try again\n");
                break;
        }

    }

    CLI_End();
    return 0;
}
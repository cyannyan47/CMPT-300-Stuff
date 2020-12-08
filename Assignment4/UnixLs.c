#include "UnixPrint.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Parse flag for print settings
    PrintSetup(argc, argv);

    // Print 
    PrintManager();

    // Close
    ClosePrint();
    return 0;
}
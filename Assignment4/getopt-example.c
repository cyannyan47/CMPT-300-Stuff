#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"

int main(int argc, char *argv[])
{
    int opt;
    
    while ((opt = getopt(argc, argv, "ilR")) != -1) {
        switch (opt) {
        case 'i': printf("Case i\n"); break;
        case 'l': printf("Case l\n"); break;
        case 'R': printf("Case R\n"); break;
        default:
            fprintf(stderr, "Usage: %s [-ilR] [file...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Now optind (declared extern int by <unistd.h>) is the index of the first non-option argument.
    // If it is >= argc, there were no non-option arguments.
    printf("%d\n", optind);
    // Parsing paths
    for (int i = optind; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    // ...

    return 0;
}
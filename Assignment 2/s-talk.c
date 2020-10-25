#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
    printf ("This program was called with \"%s\".\n",argv[0]);

    if (argc != 4) {
        printf("Invalid number of arguments\n");
        printf("Please follow this format\ns-talk [my port number] [remote hostname] [remote port number]\n");
        return EXIT_FAILURE;
    }
    
    printf("My Port number: %s\n", argv[1]);
    printf("Want to connect to machine: %s\n", argv[2]);
    printf("With port: %s\n", argv[3]);

    return 0;
}
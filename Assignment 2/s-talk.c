#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <stdbool.h>

#include "UDP_Tx.h"
#include "UDP_Rx.h"

#define MAXLENGTH 1024
int Kb_in(char *msg_to);
void UDP_Tx();
void UDP_Rx();
void Scr_out();
bool terminate_sequence(char *ch, bool *term_seq0, bool *term_seq1);

int main (int argc, char *argv[])
{
    printf ("This program was called with \"%s\".\n",argv[0]);

    if (argc != 4) {
        printf("How to use:\ns-talk [local port number] [remote hostname] [remote port number]\n");
        return EXIT_FAILURE;
    }
    
    printf("My Port number: %s\n", argv[1]);
    printf("Want to connect to machine: %s\n", argv[2]);
    printf("With port: %s\n", argv[3]);

    
    return 0;
}

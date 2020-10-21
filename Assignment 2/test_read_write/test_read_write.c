#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

bool terminate_sequence(char *ch, bool *term_seq0, bool *term_seq1);

int main() {
    int sz;
    char ch;
    bool term_seq0, term_seq1;
    term_seq0 = true;
    term_seq1 = false;
    while (read(STDIN_FILENO, &ch, 1) > 0) {
        sz = write(STDOUT_FILENO, &ch, 1);
        if (terminate_sequence(&ch, &term_seq0, &term_seq1)) {
            break;
        }
    }
    return 0;
}

bool terminate_sequence(char *ch, bool *term_seq0, bool *term_seq1) {
    if (*ch == '!') {
        if (*term_seq0 && !*term_seq1) {
            *term_seq1 = true;
        }
        else {
            *term_seq0 = false;
            *term_seq1 = false;
        }
    }
    else if (*ch == '\n') {
        if (!*term_seq0) {
            *term_seq0 = true;
        }
        else if (*term_seq1) {
            return true;
        }
        else {
            *term_seq0 = false;
        }
    }
    else {
        *term_seq0 = false;
        *term_seq1 = false;
    }
    return false;
}

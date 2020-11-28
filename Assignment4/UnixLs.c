#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

int main(int argc, char* argv[]) {
    printf("Using UnixLs\n");
    DIR *dir;
    struct dirent *dp;

    if ((dir = opendir (".")) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }

    while ((dp = readdir (dir)) != NULL) {
        printf("%s\n", dp->d_name);
    }

    closedir(dir);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>

int main(int argc, char* argv[]) {
    printf("Using UnixLs\n");
    DIR *dir;
    struct dirent *dp;
    struct stat filestat;

    if ((dir = opendir (".")) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }

    // Normal ls
    while ((dp = readdir (dir)) != NULL) {
        stat(dp->d_name, &filestat);
        if( S_ISDIR(filestat.st_mode) )
            printf("%4s: %s\n","Dir",dp->d_name);
        else
            printf("%4s: %s\n","File",dp->d_name);

        // ls -i
        printf("%4s %4lu\n", "File inode", filestat.st_ino);
        printf("%4s %4u\n", "File r/w/e permission", filestat.st_mode);
        printf("%4s %4lu\n", "Hard links num", filestat.st_nlink);
        printf("%4s %4u\n", "username", filestat.st_uid);
        printf("%4s %4u\n", "groupname", filestat.st_gid);
        printf("%4s %4lu\n", "File size", filestat.st_size);
        printf("%4s %4lu\n", "File date and time", filestat.st_mtime);
        printf("---------------------------------\n");
    }

    // ls -l
    // ls -R

    closedir(dir);
    return 0;
}
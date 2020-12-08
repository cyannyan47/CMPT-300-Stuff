#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <time.h>

int main(int argc, char* argv[]) {
    printf("Using UnixLs\n");
    DIR *dir;
    struct dirent *dp;
    struct stat filestat;
    time_t t;
    struct tm lt;

    printf("Argc: %d\n", argc);
    for (int i = 1 ; i < argc ; i++) {
        printf("PATH argument: %s\n", argv[i]);
        stat(argv[i], &filestat);
        if( S_ISDIR(filestat.st_mode) )
            printf("%4s: %s\n","Dir",argv[i]);
        else
            printf("%4s: %s\n","Not Dir",argv[i]);
    }
    

    if ((dir = opendir (argv[1])) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }

    // Normal ls
    while ((dp = readdir (dir)) != NULL) {
        lstat(dp->d_name, &filestat);
        if( S_ISDIR(filestat.st_mode) )
            printf("%4s: %s\n","Dir",dp->d_name);
        else
            printf("%4s: %s\n","File",dp->d_name);

        // ls -i
        printf("%4s %4lu\n", "File inode", filestat.st_ino);
        // ls -l
        printf("%4s %4u\n", "File r/w/e permission", filestat.st_mode);
        printf("%4s %4lu\n", "Hard links num", filestat.st_nlink);
        printf("%4s %4u\n", "username", filestat.st_uid);
        printf("%4s %4u\n", "groupname", filestat.st_gid);
        printf("%4s %4lu\n", "File size", filestat.st_size);
        printf("%4s %4lu\n", "File date and time", filestat.st_mtime);
        t = filestat.st_mtime;
        localtime_r(&t, &lt);
        char timbuf[80];
        strftime(timbuf, sizeof(timbuf), "%b %d %Y %H:%M", &lt);
        printf("Time: %s\n", timbuf);
        printf("---------------------------------\n");
    }

    // ls -l
    // ls -R

    closedir(dir);
    return 0;
}
#define _DEFAULT_SOURCE
#include <time.h>
#include "UnixPrint.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <stdbool.h>
#include <unistd.h>

// Group/username
#include <pwd.h>
#include <grp.h>
#include <string.h>

#include "list.h"

static int currentIndex = 0;
static bool l_flag = false;
static bool R_flag = false;
static bool i_flag = false;
static bool showCurrDir = false;

// IS a stack according to multiple paths ls print order
static List* pathsList;
static void pathsListFree(void *pPath) {
    free(pPath);
}

void printGroupName(gid_t grpNum);
void printUserName(uid_t uid);
void printLastModifiedTime(time_t t);


void PrintSetup(int argc, char* argv[]) {
    int opt;
    pathsList = List_create();
    // Parsing flags
    while ((opt = getopt(argc, argv, "ilR")) != -1) {
        switch (opt) {
        case 'i': i_flag = true; break;
        case 'l': l_flag = true; break;
        case 'R': R_flag = true; break;
        default:
            fprintf(stderr, "Usage: %s [-ilR] [file...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    for (currentIndex = optind; currentIndex < argc; currentIndex++) {
        // Assuming there are no flags after path arguments
        List_append(pathsList, argv[currentIndex]);
    }

    if (List_count(pathsList) == 0) {
        List_append(pathsList, ".");
    }
}

void PrintManager() {
    while (List_count(pathsList) != 0) {
        // Get path from stack
        char* path = List_trim(pathsList);
        // Check if path is file or symlink file
        PrintFileInfo(path);

        // Path is directory or symlink directory


        // Get path from list
    }
    

}

void ClosePrint() {
    List_free(pathsList, pathsListFree);
}

void PrintDir(char* path) {
    //Initializing variables
    DIR *dir;
    struct dirent *dp;

    if ((dir = opendir(path)) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }
    while ((dp = readdir (dir)) != NULL) {
        PrintFileInfo(dp->d_name);
    }
    closedir(dir);
}

void PrintFileInfo(char* path) {
    //Initializing variables
    struct stat filestat;
    lstat(path, &filestat);

    // If i flag is set
    if (i_flag) {
        printf("%-20lu", filestat.st_ino);

        printf(" ");    // Spacing
    }

    // If l flag is set
    if (l_flag) {
        // File Permissions
        if (S_ISDIR(filestat.st_mode)) {
            printf("d");
        }
        else if (S_ISDIR(filestat.st_mode)) {
            printf("l");
        }
        else {
            printf("-");
        }
        printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
        printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");

        printf(" ");    // Spacing

        // number of hard links
        printf("%lu", filestat.st_nlink);
        printf(" ");    // Spacing

        // user name
        printUserName(filestat.st_uid);
        printf(" ");    // Spacing

        // group name
        printGroupName(filestat.st_gid);
        printf(" ");    // Spacing

        // file size
        printf("%-10lu", filestat.st_size);
        printf(" ");    // Spacing

        // lastest modified time
        printLastModifiedTime(filestat.st_mtime);
        printf(" ");    // Spacing
    }

    printf("%s\n", path);
}

void printGroupName(gid_t grpNum) {
    struct group *grp;

    grp = getgrgid(grpNum); 
    int len = strlen(grp->gr_name);
    //printf("strlen %zu", strlen(grp->gr_name));
    printf("%-.*s", len, grp->gr_name);
}

void printUserName(uid_t uid) {

    struct passwd *pw = NULL;
    pw = getpwuid(uid);
    int len = strlen(pw->pw_name);
    printf("%-.*s", len, pw->pw_name);
}

void printLastModifiedTime(time_t t) {
    struct tm lt;
    localtime_r(&t, &lt);
    char timbuf[80];
    strftime(timbuf, sizeof(timbuf), "%b %e %Y %H:%M", &lt);
    printf("%s", timbuf);
}
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
static bool showBaseDir = false;

// IS a stack according to multiple paths ls print order
static List* pathsList;
static void pathsListFree(void *pPath) {
    free(pPath);
}

void printGroupName(gid_t grpNum);
void printUserName(uid_t uid);
void printLastModifiedTime(time_t t);
void PrintDir(char* path);
void PrintFileInfo(char* path, char* filename);

static bool isDir(char* path) {
    struct stat pathstat;
    lstat(path, &pathstat);
    if (S_ISDIR(pathstat.st_mode)) {
        return true;
    }
    return false;
}

static bool isLink(char* path) {
    struct stat pathstat;
    lstat(path, &pathstat);
    if (S_ISLNK(pathstat.st_mode)) {
        return true;
    }
    return false;
}


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
    
    int numOfDir = 0;

    for (currentIndex = optind; currentIndex < argc; currentIndex++) {
        // Assuming there are no flags after path arguments
        if (isDir(argv[currentIndex])) {
            numOfDir++;
        }
        List_append(pathsList, argv[currentIndex]);
    }

    if (List_count(pathsList) == 0) {
        List_append(pathsList, ".");
    }

    if (numOfDir > 1) {
        showBaseDir = true;
    }
}

void PrintManager() {
    while (List_count(pathsList) != 0) {
        // Get path from stack
        char* path = List_trim(pathsList);

        if (isLink(path)) {
            if (l_flag) {
                goto normal;
            }
            struct stat beginstat;
            stat(path, &beginstat);
            if (S_ISDIR(beginstat.st_mode)) {
                PrintDir(path);
            } else {
                PrintFileInfo(path, NULL);
            }
        }

        normal:
        // Path is directory or symlink directory
        if (isDir(path)) {
            PrintDir(path);
        } else {
            PrintFileInfo(path, NULL);
        }
        
    }
}

void ClosePrint() {
    List_free(pathsList, pathsListFree);
}

void PrintDir(char* path) {
    //Initializing variables
    DIR *dir;
    struct dirent *dp;

    // Printing
    if ((dir = opendir(path)) == NULL) {
        perror ("Cannot open .");
        exit (1);
    }
    if (R_flag) {
        printf("%s:\n", path);
    } else {
        if (showBaseDir) {
            printf("%s:\n", path);
        }
    }
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.') {
            continue;
        }
        // Check if path has '/' add a back
        int pathLen = strlen(path);
        char tempPath[pathLen + 2];
        if (path[pathLen] != '/') {
            pathLen += 1;
            strcpy(tempPath, path);
            strcat(tempPath, "/");
        }
        int dpLen = strlen(dp->d_name);
        int totalLen = pathLen + dpLen;

        char fullPath[totalLen + 1];
        strcpy(fullPath, tempPath);
        strcat(fullPath, dp->d_name);
        PrintFileInfo(fullPath, dp->d_name);
    }
    printf("\n");
    closedir(dir);

    // Pre-order traversal when R_flag is set
    if (R_flag) {
        if ((dir = opendir(path)) == NULL) {
            perror ("Cannot open .");
            exit (1);
        }
        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] == '.') {
                continue;
            }
            // Check if path has '/' add a back
            int pathLen = strlen(path);
            char tempPath[pathLen + 2];
            if (path[pathLen] != '/') {
                pathLen += 1;
                strcpy(tempPath, path);
                strcat(tempPath, "/");
            }
            int dpLen = strlen(dp->d_name);
            int totalLen = pathLen + dpLen;

            char fullPath[totalLen + 1];
            strcpy(fullPath, tempPath);
            strcat(fullPath, dp->d_name);
            
            if (isDir(fullPath)) {
                PrintDir(fullPath);
            }

        }
        closedir(dir);
    }
}

void PrintFileInfo(char* path, char* filename) {
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
        else if (S_ISLNK(filestat.st_mode)) {
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
        int len = snprintf(NULL, 0, "%lu", filestat.st_nlink);
        printf("%-.*lu", len, filestat.st_nlink);
        printf(" ");    // Spacing

        // user name
        printUserName(filestat.st_uid);
        printf(" ");    // Spacing

        // group name
        printGroupName(filestat.st_gid);
        printf(" ");    // Spacing

        // file size
        len = snprintf(NULL, 0, "%lu", filestat.st_size);
        printf("%-.*lu", len, filestat.st_size);
        printf(" ");    // Spacing

        // lastest modified time
        printLastModifiedTime(filestat.st_mtime);
        printf(" ");    // Spacing
    }
    
    if (S_ISLNK(filestat.st_mode)) {
        char* linkname = malloc(filestat.st_size + 1);
        readlink(path, linkname, filestat.st_size + 1);
        linkname[filestat.st_size] = '\0';
        if (filename) {
            printf("%s -> %s\n", filename, linkname);
        } else {
            printf("%s -> %s\n", path, linkname);
        }
        
    } else {
        if (filename) {
            printf("%s\n", filename);
        } else {
            printf("%s\n", path);
        }
    }
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
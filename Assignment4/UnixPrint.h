#ifndef UNIXPRINT_H
#define UNIXPRINT_H

// Assuming the flags come before path(s) argument
void PrintSetup(int argc, char* argv[]);
void PrintDir(char* path);
void PrintFileInfo(char* path);
void PrintManager();
void ClosePrint();
#endif
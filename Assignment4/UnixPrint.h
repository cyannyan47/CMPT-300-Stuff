#ifndef UNIXPRINT_H
#define UNIXPRINT_H

// Assuming the flags come before path(s) argument
void PrintSetup(int argc, char* argv[]);
void PrintManager();
void ClosePrint();
#endif
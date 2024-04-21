#ifndef __PRINTS_H__
#define __PRINTS_H__

#include <stdio.h>

void printActions();
void printModifyingOptions();
void printDisplayOptions();
void waitingForQuit();
void printERANGE();
void printSuccess();
void printSearchOptions();
void printHelpMenu();
void getString(char *str, int bufsiz);
double getDouble(double min, double max);

#endif // __PRINTS_H__
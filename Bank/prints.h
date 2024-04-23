#ifndef __PRINTS_H__
#define __PRINTS_H__

#include <stdio.h>
#include <stdbool.h>
#include "typedefs.h"

void printActions();
void printModifyingOptions();
void printDisplayOptions();
void waitingForQuit();
void printERANGE();
void printSuccess();
void printSearchOptions();
void printHelpMenu();
void printLine();
void printAllList();
void printAccount(account_t acc);
void printAccounts(Fixed_string key, bool (*condition)(account_t ref, Fixed_string key));
void getString(char *str, int bufsiz);
double getDouble(double min, double max, const char *msg);

#endif // __PRINTS_H__
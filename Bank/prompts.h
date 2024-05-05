#ifndef __PROMPTS_H__
#define __PROMPTS_H__

#include "typedefs.h"
#include <stdbool.h>
#include <stdio.h>

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
void printAccount(Account_t acc);
void printAccounts(Fixed_string key, bool (*condition)(Account_t ref, Fixed_string key));
void getString(char *str, int bufsiz, const char *msg, bool clear);
bool checkDigits(char *buff);
double getDouble(double min, double max, const char *msg);

#endif // __PRINTS_H__
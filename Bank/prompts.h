#ifndef __PROMPTS_H__
#define __PROMPTS_H__

#include "typedefs.h"
#include <stdbool.h>
#include <stdio.h>

void printActions();
void printModifyingOptions();
void printDisplayOptions();
void waitingForQuit();
void printOutOfRange();
void printSuccess();
void printAbort();
void printSearchOptions();
void printHelpMenu();
void printLine();
void printAllList();
void printListHeader();
void printAccount(Account_t acc);
void printAccounts(Fixed_string key, bool (*condition)(Account_t ref, Fixed_string key));
void getString(char *str, int bufsiz, const char *msg, bool clear);
bool checkDigits(char *str);
bool checkLetters(char *str);
bool noLetters(char* str);
double getDouble(double min, double max, const char *msg);

#endif // PROMPTS_H__
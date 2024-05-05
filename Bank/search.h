#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "typedefs.h"
#include <stdbool.h>

void searchList();
void getSearchKey(char *search_key, short len);
bool findName(Account_t ref, Fixed_string key);
bool findSurname(Account_t ref, Fixed_string key);
bool findAddress(Account_t ref, Fixed_string key);
bool findPESEL(Account_t ref, Fixed_string key);
bool findAccountNumber(Account_t ref, Fixed_string key);

#endif // __SEARCH_H__
#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "typedefs.h"
#include <stdbool.h>

void searchList();
void getSearchKey(char *search_key, short len);
bool findName(account_t ref, Fixed_string key);
bool findSurname(account_t ref, Fixed_string key);
bool findAddress(account_t ref, Fixed_string key);
bool findPESEL(account_t ref, Fixed_string key);
bool findAccountNumber(account_t ref, Fixed_string key);

#endif // __SEARCH_H__
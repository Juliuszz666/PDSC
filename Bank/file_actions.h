#ifndef __FILE_ACTIONS_H__
#define __FILE_ACTIONS_H__

#include "typedefs.h"
#include <stdbool.h>

bool confirmation(Account_t accounts[], bool is_transfer);
Account_t findAccount(const char *msg, bool *found);
void updateTransfer(Account_t source, Account_t destination);
void updateAccount(Account_t updated);

#endif // __FILE_ACTIONS_H__
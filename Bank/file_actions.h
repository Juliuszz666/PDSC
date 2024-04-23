#ifndef __FILE_ACTIONS_H__
#define __FILE_ACTIONS_H__

#include "typedefs.h"
#include <stdbool.h>

account_t findAccount(const char *msg, bool *found);
void updateTransfer(account_t source, account_t destination);
void updateAccount(account_t updated);

#endif // __FILE_ACTIONS_H__
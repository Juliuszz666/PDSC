#ifndef __ACC_CREATION_H__
#define __ACC_CREATION_H__

#include "typedefs.h"
#include <stdbool.h>

void getPESEL(account_t *newaccount);
void getName(account_t *newaccount);
void getLocation(account_t *newaccount);
void getBalance(account_t *newaccount);
void getLoanInfo(account_t *newaccount);
void generateIBAN(account_t *newaccount);
bool isIBANoverlapping(IBAN check_val);

#endif // __ACC_CREATION_H__
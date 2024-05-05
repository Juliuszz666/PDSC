#ifndef __ACC_CREATION_H__
#define __ACC_CREATION_H__

#include "typedefs.h"
#include <stdbool.h>

void getPESEL(Account_t *newaccount);
void getName(Account_t *newaccount);
void getLocation(Account_t *newaccount);
void getBalance(Account_t *newaccount);
void getLoanInfo(Account_t *newaccount);
void generateIBAN(Account_t *newaccount);
bool isIBANoverlapping(IBAN check_val);

#endif // __ACC_CREATION_H__
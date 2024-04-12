#ifndef __DEFS_H__
#define __DEFS_H__

#define IBAN_LENGTH 26
#define PESEL_LENGTH 11

#define CHARBUFFER 20
#define ADDRBUFFER 56
#define DATA_COUNT 7

#define CASH_MAX 999999999999999.0
#define CASH_MIN -99999999999999.0

#define LINE_LENGTH 203

typedef char IBAN[IBAN_LENGTH + 1];
typedef char PESEL[PESEL_LENGTH + 1];
typedef char Fixed_string[CHARBUFFER];
typedef char Address[ADDRBUFFER];

#endif //__DEFS_H__
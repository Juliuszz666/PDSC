#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdint.h>

#pragma pack(1)

#define DATA_FILE "clients.dat"

#define BUFFER 256

#define IBAN_LENGTH 26
#define PESEL_LENGTH 11

#define ID_LEN 7
#define CHARBUFFER 20
#define ADDRBUFFER 56
#define DATA_COUNT 7
#define BALANCE_INFO_NO 3
#define BALANCE_SIZE_C 19
#define LOAN_SIZE_C 14
#define INTERESET_SIZE_C 12

#define CASH_MAX 99999999999999999.0
#define LOAN_MAX 999999999999.0
#define CASH_MIN -0.01

#define COUNTRY "PL69"
#define BANK_CODE "211569420"

#define PRECISION 2
#define MAX_INTERESET 1.00
#define BANK_INTEREST 0.05
#define MONTHS_OF_PAYMENT 360

#define LINE_LENGTH 213

typedef char IBAN[IBAN_LENGTH + 1];
typedef char PESEL[PESEL_LENGTH + 1];
typedef char Fixed_string[CHARBUFFER];
typedef char Address[ADDRBUFFER];

typedef struct
{
    uint32_t id;
    IBAN account_number;
    PESEL pesel_number;
    Fixed_string first_name;
    Fixed_string last_name;
    Address address;
    double balance;
    double bank_loan;
    double interest;
} Account_t;

#endif //__DEFS_H__
#ifndef __LIST_H__
#define __LIST_H__

#pragma pack(1)

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IBAN_LENGTH 26
#define PESEL_LENGTH 11

#define CHARBUFFER 50

typedef char IBAN[IBAN_LENGTH + 1];
typedef char PESEL[PESEL_LENGTH + 1];
typedef char fixed_string[CHARBUFFER];

typedef struct list
{
    IBAN account_number;
    PESEL pesel_number;
    fixed_string first_name;
    fixed_string last_name;
    fixed_string addres;
    double balance;
    double bank_loan;
    double interest;
    struct list *next;
    struct list *prev;
} node;

node *createNode(IBAN, fixed_string first_name, fixed_string surname, fixed_string address, PESEL,
                 double balance, double loan, double interest);
void pushNode(node **, node *);
void removeNode();
void printList();

#endif // __LIST_H__
#ifndef __LIST_H__
#define __LIST_H__

#pragma pack(1)

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IBAN_LENGTH 26
#define PESEL_LENGTH 11

typedef char IBAN[IBAN_LENGTH + 1];
typedef char PESEL[PESEL_LENGTH + 1];

typedef struct
{
    char *first_name;
    char *surname;
} name;
typedef struct
{
    char *country;
    char *region;
    char *city;
    char *street;
    char *st_number;
    char *apart_num;
} location;

typedef struct list
{
    IBAN account_number;
    PESEL pesel_number;
    name full_name;
    location addres;
    double balance;
    double bank_loan;
    double interest;
    struct list *next;
    struct list *prev;
} node;

node *createNode(IBAN, name, location, PESEL, double, double, double);
void pushNode(node **, node *);
void removeNode();
void printList();

#endif // __LIST_H__
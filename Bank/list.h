#ifndef __LIST_H__
#define __LIST_H__

#pragma pack(1)

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "typedefs.h"

typedef struct list
{
    IBAN account_number;
    PESEL pesel_number;
    Fixed_string first_name;
    Fixed_string last_name;
    Address address;
    double balance;
    double bank_loan;
    double interest;
    struct list *next;
} node;

node *createNode(IBAN, Fixed_string first_name, Fixed_string surname, Address address, PESEL,
                 double balance, double loan, double interest);
void pushNode(node **cur_head, node *new_head);
void removeNode();
node *searchForNode(node **head, void* key, bool (*compare)(void *key, node *ref));
void deleteList(node **head);
void printList(node *head);

#endif // __LIST_H__
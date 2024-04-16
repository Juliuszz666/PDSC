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
node *searchForNode(node **head, void *key, bool (*compare)(void *key, node *ref));
void deleteList(node **head);
void printList(node *head, bool (*condition)(const char *key, node *ref), char *key);
void printAllList(node *head);
void searchList(node *head);

void getSearchKey(char *search_key, short len);

bool findName(const char *key, node *ref);
bool findSurname(const char *key, node *ref);
bool findAddress(const char *key, node *ref);
bool findPESEL(const char *key, node *ref);
bool findAccountNumber(const char *key, node *ref);

#endif // __LIST_H__
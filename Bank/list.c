#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "list.h"

node *createNode(IBAN iban, name input_name, location input_address, PESEL input_pesel,
                 double input_balance, double input_loan, double input_interest)
{
    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->account_number, iban);
    new_node->full_name = input_name;
    new_node->addres = input_address;
    strcpy(new_node->pesel_number, input_pesel);
    new_node->balance = input_balance;
    new_node->bank_loan = input_loan;
    new_node->interest = input_interest;

    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}
void pushNode(node **cur_head, node *new_head)
{
    new_head->prev = NULL;
    new_head->next = (*cur_head);
    if ((*cur_head) != NULL)
    {
        (*cur_head)->prev = new_head;
    }
    (*cur_head) = new_head;
}

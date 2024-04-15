#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "list.h"
#include "prints.h"

node *createNode(IBAN iban, Fixed_string first_name, Fixed_string surname, Address address,
                 PESEL input_pesel, double input_balance, double input_loan, double input_interest)
{
    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->account_number, iban);
    strcpy(new_node->first_name, first_name);
    strcpy(new_node->last_name, surname);
    strcpy(new_node->address, address);
    strcpy(new_node->pesel_number, input_pesel);
    new_node->balance = input_balance;
    new_node->bank_loan = input_loan;
    new_node->interest = input_interest;

    new_node->next = NULL;

    return new_node;
}
void pushNode(node **cur_head, node *new_head)
{
    if (*cur_head == NULL)
    {
        *cur_head = new_head;
    }
    else
    {
        new_head->next = *cur_head;
        *cur_head = new_head;
    }
}
void deleteList(node **head)
{
    node *temp = *head;
    while (temp != NULL)
    {
        node *next = temp->next;
        free(temp);
        temp = next;
    }
    *head = NULL;
}

void printLine()
{
    for (int i = 0; i < LINE_LENGTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

void printList(node *head, bool (*condition)(const char *key, node *ref), char *key)
{
    system("clear");
    node *temp = head;
    printLine();
    printf("| %-26s | %-20s | %-20s | %-56s | %-11s | %-18s | %-18s | %-12s |\n", "Account Number",
           "First Name", "Last Name", "Address", "PESEL", "Balance", "Bank Loan", "Interest");
    printLine();
    bool is_condition = (condition != NULL);
    while (temp != NULL)
    {
        if ((is_condition && (*condition)(key, temp)) || !is_condition)
        {
            printf("| %-26s | %-20s | %-20s | %-56s | %-11s | %-18.2f | %-18.2f | %-12.2f |\n",
                   temp->account_number, temp->first_name, temp->last_name, temp->address,
                   temp->pesel_number, temp->balance, temp->bank_loan, temp->interest);
            printLine();
        }
        temp = temp->next;
    }
    waitingForQuit();
}
node *searchForNode(node **head, void *key, bool (*compare)(void *key, node *ref))
{
    node *temp = *head;
    while (temp != NULL && (*compare)(key, temp) != 0)
    {
        temp = temp->next;
    }
    return temp;
}
void printAllList(node *head)
{
    printList(head, NULL, NULL);
}
bool findName(const char *key, node *ref)
{
    return (strstr(ref->first_name, key) != NULL);
}
bool findSurname(const char *key, node *ref)
{
    return (strstr(ref->last_name, key) != NULL);
}
bool findAddress(const char *key, node *ref)
{
    return (strstr(ref->address, key) != NULL);
}
bool findPESEL(const char *key, node *ref)
{
    return (strstr(ref->pesel_number, key) != NULL);
}
bool findAccountNumber(const char *key, node *ref)
{
    return (strstr(ref->account_number, key) != NULL);
}
void searchList(node *head)
{
    printSearchOptions();
    bool (*searchFun)(const char *key, node *ref);
    bool isIBAN = false;
    Fixed_string search_type;
    Fixed_string search_key;
    IBAN iban_key;
    fgets(search_type, sizeof(search_type), stdin);
    search_type[strcspn(search_type, "\n")] = '\0';
    if (strcmp(search_type, "acc num") == 0)
    {
        searchFun = &findAccountNumber;
        isIBAN = true;
    }
    else if (strcmp(search_type, "fname") == 0)
    {
        searchFun = &findName;
    }
    else if (strcmp(search_type, "lname") == 0)
    {
        searchFun = &findSurname;
    }
    else if (strcmp(search_type, "addr") == 0)
    {
        searchFun = &findAddress;
    }
    else if (strcmp(search_type, "pesel") == 0)
    {
        searchFun = &findPESEL;
    }
    else
    {
        printf("Invalid search key\n");
        waitingForQuit();
        return;
    }
    getSearchKey(search_key, iban_key,isIBAN);
    printList(head, searchFun, search_key);
}
void getSearchKey(Fixed_string search_key, IBAN iban_key, bool isIBAN)
{
    system("clear");
    printf("Enter search key: ");
    fgets(isIBAN ? iban_key : search_key, isIBAN ? IBAN_LENGTH : CHARBUFFER, stdin);
    if(isIBAN) iban_key[strcspn(iban_key, "\n")] = '\0';
    else search_key[strcspn(search_key, "\n")] = '\0';
}
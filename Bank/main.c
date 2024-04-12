#include <assert.h>
#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"
#include "prints.h"

#define COUNTRY "PL69"
#define BANK_CODE "211569420"

#define BALANCE_INFO_NO 3

void loadData();
void saveData();

void chooseAction();
void chooseModifyingOperation();
void chooseDisplayOperation();
void generateIBAN(IBAN, node *);
bool isIBANoverlapping(node *, IBAN);
bool confimationOfAction(int);

void getName(Fixed_string name, Fixed_string surname);
void getPESEL(PESEL pesel);
void getLocation(Address location);

void createAccount();
void makeDeposit();
void makeWithdrawal();
void transferMoney();
void takeLoan();
void payDebt();

node *head = NULL;

int main(int argc, char const *argv[])
{
    while (1)
    {
        chooseAction();
    }

    return 0;
}
void loadData()
{
    FILE *file = fopen("data.txt", "r");
    if (file == NULL)
    {
        printf("No data to load.\n");
        return;
    }
    IBAN acc_num;
    PESEL id_number;
    Fixed_string first_name, surname;
    Address address;
    double balance_info[BALANCE_INFO_NO];
    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf\n", acc_num, first_name, surname,
                  address, id_number, &balance_info[0], &balance_info[1], &balance_info[2]) != EOF)
    {
        node *newAccount = createNode(acc_num, first_name, surname, address, id_number,
                                      balance_info[0], balance_info[1], balance_info[2]);
        pushNode(&head, newAccount);
    }
    fclose(file);
}
void saveData()
{
    FILE *file = fopen("data.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    node *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%s|%s|%s|%s|%s|%.2lf|%.2lf|%.2lf\n", temp->account_number, temp->first_name,
                temp->last_name, temp->address, temp->pesel_number, temp->balance, temp->bank_loan,
                temp->interest);
        temp = temp->next;
    }
    fclose(file);
}
void chooseAction()
{
    printActions();
    int key = getchar();
    getchar();
    switch (key)
    {
    case '1':
        chooseModifyingOperation();
        break;
    case '2':
        chooseDisplayOperation();
        break;
    default:
        break;
    }
}

void chooseModifyingOperation()
{
    void (*functionPointer)(void);
    printModifyingOptions();
    int key = getchar();
    getchar();

    switch (key)
    {
    case '1':
        functionPointer = &createAccount;
        break;
    case '2':
        functionPointer = &makeDeposit;
        break;
    case '3':
        functionPointer = &makeWithdrawal;
        break;
    case '4':
        functionPointer = &transferMoney;
        break;
    case '5':
        functionPointer = &takeLoan;
        break;
    case '6':
        functionPointer = &payDebt;
        break;
    default:
        printf("Invalid operation\n");
        break;
    }
    if (confimationOfAction(key - '0') && key != -1)
    {
        (*functionPointer)();
    }
}
bool confimationOfAction(int action_no)
{
    system("clear");
    printf("Do you want to performs this action number %d?\nPress Y/y if yes, otherwise press "
           "anything else\n",
           action_no);
    int action = getchar();
    getchar();
    return (action == 'y' || action == 'Y');
}
void createAccount()
{
    loadData();
    IBAN acc_num;
    PESEL id_number;
    Fixed_string first_name, surname;
    Address address;
    generateIBAN(acc_num, head);
    getPESEL(id_number);
    getName(first_name, surname);
    getLocation(address);
    double balance_info[BALANCE_INFO_NO] = {0};
    // getBalanceInfo(balance_info);
    node *newAccount = createNode(acc_num, first_name, surname, address, id_number, balance_info[0],
                                  balance_info[1], balance_info[2]);
    pushNode(&head, newAccount);
    saveData();
    deleteList(&head);
}
void getPESEL(PESEL pesel)
{
    Fixed_string buffer;
    do
    {
        system("clear");
        printf("Enter your PESEL: ");
        fgets(buffer, sizeof(Fixed_string), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    } while (strlen(buffer) != PESEL_LENGTH);
    strcpy(pesel, buffer);
}
void getName(Fixed_string first_name, Fixed_string surname)
{
    system("clear");
    printf("Enter first name: ");
    fgets(first_name, sizeof(Fixed_string), stdin);
    first_name[strcspn(first_name, "\n")] = '\0';
    system("clear");
    printf("Enter surname: ");
    fgets(surname, sizeof(Fixed_string), stdin);
    surname[strcspn(surname, "\n")] = '\0';
}
void getLocation(Address location)
{
    system("clear");
    printf("Enter your address: ");
    fgets(location, sizeof(Address), stdin);
    location[strcspn(location, "\n")] = '\0';
    printf("%s\n", location);
    printf("%ld\n", strlen(location));
}
bool compareIBAN(void *key, node *ref)
{
    return (memcmp(key, ref->account_number, IBAN_LENGTH));
}
void makeDeposit()
{
    loadData();
    node *action_node = NULL;
    system("clear");
    printf("Provide account number: ");
    IBAN action_account;
    fgets(action_account, IBAN_LENGTH + 1, stdin);
    action_node = searchForNode(&head, action_account, compareIBAN);
    if (action_node == NULL)
    {
        printf("Invalid account number!\n");
    }
    else
    {
        char quit;
        printf("Provide amount you want to deposit\n");
        Fixed_string buffer;
        while (getchar() != '\n');
        fgets(buffer, CHARBUFFER, stdin);
        double deposit = strtod(buffer, NULL);
        if (CASH_MIN >= (deposit + action_node->balance) ||
            CASH_MAX <= (deposit + action_node->balance))
        {
            errno = ERANGE;
            perror("Value overflow, operation terminated");
            while ((quit = getchar()) != 'q' && quit != 'Q');
        }
        else if (deposit <= 0)
        {
            errno = ERANGE;
            perror("Deposit amount must be positive");
            while ((quit = getchar()) != 'q' && quit != 'Q');
        }
        else
        {
            action_node->balance += deposit;
            printf("Operation sucessful\nPress q/Q to continue\n");
            while ((quit = getchar()) != 'q' && quit != 'Q');
        }
    }
    saveData();
    deleteList(&head);
}
void makeWithdrawal()
{
    printf("GIIT\n");
}
void transferMoney()
{
    printf("GIIT\n");
}
void takeLoan()
{
    printf("GIIT\n");
}
void payDebt()
{
    printf("GIIT\n");
}
void chooseDisplayOperation()
{
    void (*functionPointer)(node *);
    system("clear");
    printDisplayOptions();
    int key = getchar();
    getchar();
    loadData();
    switch (key)
    {
    case '1':
        functionPointer = &printList;
        break;
    case '2':
        // functionPointer = &makeDeposit;
        break;
    default:
        printf("Invalid operation\n");
        break;
    }
    (*functionPointer)(head);
    deleteList(&head);
}
void generateIBAN(IBAN to_be_generated, node *head)
{
    do
    {
        srand(time(NULL));
        int iban_ptr = 0;
        assert((iban_ptr + strlen(COUNTRY)) < IBAN_LENGTH);
        strcpy(to_be_generated, COUNTRY);
        iban_ptr += strlen(COUNTRY);
        assert((iban_ptr + strlen(BANK_CODE)) < IBAN_LENGTH);
        strcpy(to_be_generated + iban_ptr, BANK_CODE);
        iban_ptr += strlen(BANK_CODE);
        for (size_t i = iban_ptr; i < IBAN_LENGTH; i++)
        {
            to_be_generated[i] = rand() % 10 + '0';
        }
        to_be_generated[IBAN_LENGTH] = '\0';
    } while (isIBANoverlapping(head, to_be_generated));
}
bool isIBANoverlapping(node *head, IBAN check_val)
{
    node *temp = head;
    while (temp != NULL)
    {
        if (temp->account_number == check_val)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
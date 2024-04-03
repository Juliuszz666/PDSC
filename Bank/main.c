#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "prints.h"

#define COUNTRY "PL69"
#define BANK_CODE "211569420"

#define BALANCE_INFO_NO 3

void chooseAction();
void chooseModifyingOperation();
void chooseDisplayOperation();
void generateIBAN(IBAN, node *);
bool isIBANoverlapping(node *, IBAN);
bool confimationOfAction(int);

name getName();
location getLocation();

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
        break;
    }

    return 0;
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
    IBAN acc_num;
    generateIBAN(acc_num, head);
    name acc_name = getName();
    location address = getLocation();
    PESEL id_number = getPESEL();
    double balance_info[BALANCE_INFO_NO];
    getBalanceInfo(balance_info);
    node *newAccount = createNode(acc_num, acc_name, address, id_number, balance_info[0],
                                  balance_info[1], balance_info[2]);
    pushNode(&head, newAccount);
    saveData();
}
name getName()
{

}
location getLocation()
{

}
void makeDeposit()
{
    printf("GIIT33\n");
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
    printDisplayOptions();
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
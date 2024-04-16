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
#define INTEREST_RATE 0.069
#define MOHTNS_OF_PAYMENT 180

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
double getMoneyValue(const char *msg);
node *findAccount(const char *msg);

void createAccount();
void makeDeposit();
void makeWithdrawal();
void transferMoney();
void takeLoan();
void payDebt();

int getAction()
{
    int key = getchar();
    while (getchar() != '\n')
        ;
    return key;
}

node *head = NULL;
char quit_flag = 0;

int main(int argc, char const *argv[])
{
    while (1)
    {
        chooseAction();
        if (quit_flag)
        {
            break;
        }
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
    int key = getAction();
    switch (key)
    {
    case '1':
        chooseModifyingOperation();
        break;
    case '2':
        chooseDisplayOperation();
        break;
    case '3':
        printHelpMenu();
    case '4':
        quit_flag = 1;
        break;
    default:
        break;
    }
}

void chooseModifyingOperation()
{
    void (*functionPointer)(void);
    printModifyingOptions();
    int key = getAction();
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
        return;
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
    while (getchar() != '\n')
        ;

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
        getString(buffer, PESEL_LENGTH + 1);
    } while (strlen(buffer) != PESEL_LENGTH);
    strcpy(pesel, buffer);
}
void getName(Fixed_string first_name, Fixed_string surname)
{
    system("clear");
    printf("Enter first name: ");
    getString(first_name, CHARBUFFER);
    system("clear");
    printf("Enter surname: ");
    getString(surname, CHARBUFFER);
}
void getLocation(Address location)
{
    system("clear");
    printf("Enter your address: ");
    getString(location, ADDRBUFFER);
}
bool compareIBAN(void *key, node *ref)
{
    return (memcmp(key, ref->account_number, IBAN_LENGTH));
}
node *findAccount(const char *msg)
{
    system("clear");
    printf("Provide %saccount number: ", msg);
    IBAN action_account;
    getString(action_account, IBAN_LENGTH + 1);
    return searchForNode(&head, action_account, compareIBAN);
}
double getMoneyValue(const char *msg)
{
    system("clear");
    printf("Provide amount you want to %s\n", msg);
    Fixed_string buffer;
    getString(buffer, CHARBUFFER);
    return strtod(buffer, NULL);
}
void makeDeposit()
{
    loadData();
    node *action_node = findAccount("");
    if (action_node == NULL)
    {
        printf("Invalid account number!\n");
        goto invalid_acc;
    }
    double deposit = getMoneyValue("deposit");
    if (CASH_MAX <= (deposit + action_node->balance) || deposit <= 0)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        action_node->balance += deposit;
        printSuccess();
    }
invalid_acc:
    saveData();
    deleteList(&head);
}
void makeWithdrawal()
{
    loadData();
    node *action_node = findAccount("");
    if (action_node == NULL)
    {
        printf("Invalid account number!\n");
        waitingForQuit();
        goto invalid_acc;
    }
    double withdrawal = getMoneyValue("withdraw");
    if (0 >= (action_node->balance - withdrawal) || withdrawal >= CASH_MAX || withdrawal <= 0)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        action_node->balance -= withdrawal;
        printSuccess();
    }
invalid_acc:
    saveData();
    deleteList(&head);
}
void transferMoney()
{
    loadData();
    node *src_account = findAccount("source ");
    while (getchar() != '\n')
        ;
    node *dst_account = findAccount("destination ");
    if (src_account == NULL || dst_account == NULL)
    {
        printf("Invalid account number!\n");
        waitingForQuit();
        goto invalid_acc;
    }
    double transfer = getMoneyValue("transfer");
    if (0 >= (src_account->balance - transfer) || transfer >= CASH_MAX || transfer <= 0 ||
        (dst_account->balance + transfer) >= CASH_MAX)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        src_account->balance -= transfer;
        dst_account->balance += transfer;
        printSuccess();
    }
invalid_acc:
    saveData();
    deleteList(&head);
}
void takeLoan()
{
    loadData();
    node *action_node = findAccount("");
    if (action_node == NULL)
    {
        printf("Invalid account number!\n");
        waitingForQuit();
        goto invalid_acc;
    }
    double loan = getMoneyValue("take a loan");
    if ((action_node->balance + loan) >= CASH_MAX || loan >= CASH_MAX || loan <= 0)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        action_node->balance += loan;
        action_node->interest = (1 + INTEREST_RATE) * loan / MOHTNS_OF_PAYMENT;
        action_node->bank_loan += action_node->interest * MOHTNS_OF_PAYMENT;
        printSuccess();
    }
invalid_acc:
    saveData();
    deleteList(&head);
}
void payDebt()
{
    loadData();
    node *action_node = findAccount("");
    if (action_node == NULL)
    {
        printf("Invalid account number!\n");
        waitingForQuit();
        goto invalid_acc;
    }
    if (0 >= (action_node->balance - action_node->interest) || action_node->bank_loan == 0)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        action_node->balance -= (action_node->bank_loan >= action_node->interest)
                                    ? action_node->interest
                                    : action_node->bank_loan;
        action_node->bank_loan -= (action_node->bank_loan >= action_node->interest)
                                      ? action_node->interest
                                      : action_node->bank_loan;
        printSuccess();
    }
invalid_acc:
    saveData();
    deleteList(&head);
}
void chooseDisplayOperation()
{
    void (*functionPointer)(node *);
    system("clear");
    printDisplayOptions();
    int key = getAction();

    switch (key)
    {
    case '1':
        functionPointer = &printAllList;
        break;
    case '2':
        functionPointer = &searchList;
        break;
    default:
        printf("Invalid operation\n");
        return;
    }
    loadData();
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
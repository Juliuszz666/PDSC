#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "prints.h"
#include "typedefs.h"

#define DATA_FILE "clients.dat"

void getPESEL(account_t *new);
void getName(account_t *new);
void getLocation(account_t *new);
void getBalance(account_t *new);
void getLoanInfo(account_t *new);
void generateIBAN(account_t *new);
bool isIBANoverlapping(IBAN check_val);

void printAllList();
void searchList();
void printAccount(account_t account);

void createAccount();
void makeDeposit();
void makeWithdrawal();
void transferMoney();
void takeLoan();
void payDebt();

account_t findAccount(const char *msg, bool *found);
void updateTransfer(account_t source, account_t destination);
void updateAccount(account_t updated);
void getSearchKey(char *search_key, short len);

void chooseAction();
void chooseModifyingOperation();
bool confimationOfAction(int action_no);
void chooseDisplayOperation();

bool findName(account_t ref, Fixed_string key);
bool findSurname(account_t ref, Fixed_string key);
bool findAddress(account_t ref, Fixed_string key);
bool findPESEL(account_t ref, Fixed_string key);
bool findAccountNumber(account_t ref, Fixed_string key);

void printAllList();

void printAccounts(Fixed_string, bool (*condition)(account_t ref, Fixed_string key));

int getAction()
{
    int key = getchar();
    while (getchar() != '\n')
        ;
    return key;
}

char quit_flag = 0;

int main(int argc, char *argv[])
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
void printLine()
{
    for (int i = 0; i < LINE_LENGTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

void printAccount(account_t acc)
{
    printf("| %*.*u | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*f | %-*.*f | %-*.*f |\n",
           ID_LEN, ID_LEN, acc.id, IBAN_LENGTH, IBAN_LENGTH, acc.account_number, CHARBUFFER,
           CHARBUFFER, acc.first_name, CHARBUFFER, CHARBUFFER, acc.last_name, ADDRBUFFER,
           ADDRBUFFER, acc.address, PESEL_LENGTH, PESEL_LENGTH, acc.pesel_number, BALANCE_SIZE_C,
           PRECISION, acc.balance, LOAN_SIZE_C, PRECISION, acc.bank_loan, INTERESET_SIZE_C,
           PRECISION, acc.interest);
    printLine();
}
bool findName(account_t ref, Fixed_string key)
{
    return strcmp(ref.first_name, key) == 0;
}
bool findSurname(account_t ref, Fixed_string key)
{
    return strcmp(ref.last_name, key) == 0;
}
bool findAddress(account_t ref, Fixed_string key)
{
    return strcmp(ref.address, key) == 0;
}
bool findPESEL(account_t ref, Fixed_string key)
{
    return strcmp(ref.pesel_number, key) == 0;
}
bool findAccountNumber(account_t ref, Fixed_string key)
{
    return strcmp(ref.account_number, key) == 0;
}
void printAllList()
{
    printAccounts(NULL, NULL);
}
void getSearchKey(char *search_key, short len)
{
    system("clear");
    printf("Enter search key: ");
    getString(search_key, len);
}
void searchList()
{
    printSearchOptions();
    bool (*searchFun)(account_t ref, Fixed_string key);
    short len = CHARBUFFER;
    Fixed_string search_type;
    Address search_key;
    getString(search_type, CHARBUFFER);
    if (strcmp(search_type, "acc num") == 0)
    {
        searchFun = &findAccountNumber;
        len = IBAN_LENGTH + 1;
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
        len = ADDRBUFFER + 1;
    }
    else if (strcmp(search_type, "pesel") == 0)
    {
        searchFun = &findPESEL;
        len = PESEL_LENGTH + 1;
    }
    else
    {
        printf("Invalid search type\n");
        waitingForQuit();
        return;
    }
    getSearchKey(search_key, len);
    printAccounts(search_key, searchFun);
}

void printAccounts(Fixed_string key, bool (*condition)(account_t ref, Fixed_string key))
{
    FILE *print_f = fopen(DATA_FILE, "rb");
    if (print_f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    account_t print;
    system("clear");
    printLine();
    printf("| %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s | %-*.*s |\n",
           ID_LEN, ID_LEN, "ID", IBAN_LENGTH, IBAN_LENGTH, "Account Number", CHARBUFFER, CHARBUFFER,
           "First Name", CHARBUFFER, CHARBUFFER, "Last Name", ADDRBUFFER, ADDRBUFFER, "Address",
           PESEL_LENGTH, PESEL_LENGTH, "PESEL", BALANCE_SIZE_C, BALANCE_SIZE_C, "Balance",
           LOAN_SIZE_C, LOAN_SIZE_C, "Bank Loan", INTERESET_SIZE_C, INTERESET_SIZE_C, "Interest");
    printLine();
    while (fread(&print, sizeof(account_t), 1, print_f))
    {
        if (condition == NULL || (condition != NULL && condition(print, key)))
        {
            printAccount(print);
        }
    }
    fclose(print_f);
    waitingForQuit();
}

account_t findAccount(const char *msg, bool *found)
{
    account_t retval;
    uint32_t search_by;
    int check;
    do
    {
        system("clear");
        printf("Enter %s account id: ", msg);
        check = scanf("%u", &search_by);
        while (getchar() != '\n')
            ;
    } while (check != 1);
    FILE *search_f = fopen(DATA_FILE, "rb");
    if (search_f == NULL)
    {
        printf("Error opening file!\n");
        return retval;
    }
    while (fread(&retval, sizeof(account_t), 1, search_f))
    {
        if (retval.id == search_by)
        {
            fclose(search_f);
            *found = true;
            return retval;
        }
    }
    fclose(search_f);
    *found = false;
    return retval;
}
void transferMoney()
{
    bool source_found = false;
    bool destination_found = false;
    account_t source = findAccount("source ", &source_found);
    account_t destination = findAccount("destination ", &destination_found);
    if (!source_found || !destination_found)
    {
        printf("One of the accounts was not found\n");
        waitingForQuit();
        return;
    }
    double transfer = getDouble(CASH_MIN, CASH_MAX, "transfer");
    if (0 >= (source.balance - transfer) || transfer >= CASH_MAX || transfer <= 0 ||
        (destination.balance + transfer) >= CASH_MAX)
    {
        errno = ERANGE;
        printERANGE();
        return;
    }
    else
    {
        source.balance -= transfer;
        destination.balance += transfer;
        printSuccess();
    }
    updateTransfer(source, destination);
}
void makeDeposit()
{
    bool found = false;
    account_t deposit_acc = findAccount("", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    double deposit_amount = getDouble(CASH_MIN, CASH_MAX, "deposit");
    if (deposit_amount >= CASH_MAX || deposit_amount < 0 ||
        (deposit_acc.balance + deposit_amount) >= CASH_MAX)
    {
        errno = ERANGE;
        printERANGE();
        return;
    }
    else
    {
        deposit_acc.balance += deposit_amount;
        printSuccess();
    }
    updateAccount(deposit_acc);
}
void makeWithdrawal()
{
    bool found = false;
    account_t withdrawal_acc = findAccount("", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    double withdrawal_amount = getDouble(CASH_MIN, CASH_MAX, "withdraw");
    if (withdrawal_amount >= CASH_MAX || withdrawal_amount < 0 ||
        0 >= (withdrawal_acc.balance - withdrawal_amount))
    {
        errno = ERANGE;
        printERANGE();
        return;
    }
    else
    {
        withdrawal_acc.balance -= withdrawal_amount;
        printSuccess();
    }
    updateAccount(withdrawal_acc);
}
void takeLoan()
{
    bool found = false;
    account_t loan_acc = findAccount("", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    double loan = getDouble(CASH_MIN, CASH_MAX, "loan");
    if ((loan_acc.balance + loan) >= CASH_MAX || loan >= LOAN_MAX || loan <= 0)
    {
        errno = ERANGE;
        printERANGE();
        return;
    }
    else
    {
        loan_acc.balance += loan;
        loan_acc.interest = (1 + BANK_INTEREST) * loan / MONTHS_OF_PAYMENT;
        loan_acc.bank_loan += loan_acc.interest * MONTHS_OF_PAYMENT;
        printSuccess();
    }
    updateAccount(loan_acc);
}
void payDebt()
{
    bool found = false;
    account_t debt_acc = findAccount("", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    if (0 >= (debt_acc.balance - debt_acc.interest) || debt_acc.bank_loan == 0)
    {
        errno = ERANGE;
        printERANGE();
    }
    else
    {
        debt_acc.balance -=
            (debt_acc.bank_loan >= debt_acc.interest) ? debt_acc.interest : debt_acc.bank_loan;
        debt_acc.bank_loan -=
            (debt_acc.bank_loan >= debt_acc.interest) ? debt_acc.interest : debt_acc.bank_loan;
        printSuccess();
    }
    if (debt_acc.bank_loan == 0)
    {
        debt_acc.interest = 0;
    }
    updateAccount(debt_acc);
}
void updateAccount(account_t updated)
{
    FILE *update_f = fopen(DATA_FILE, "rb+");
    if (update_f == NULL)
    {
        perror("Error opening file!\n");
        return;
    }
    if (fseek(update_f, sizeof(account_t) * (updated.id - 1), SEEK_SET) != 0)
    {
        perror("Error seeking file!\n");
        fclose(update_f);
        return;
    }
    fwrite(&updated, sizeof(account_t), 1, update_f);
    fclose(update_f);
}
void updateTransfer(account_t source, account_t destination)
{
    updateAccount(source);
    updateAccount(destination);
}

void getPESEL(account_t *new)
{
    PESEL buffer;
    do
    {
        system("clear");
        printf("Enter your PESEL: ");
        getString(buffer, PESEL_LENGTH + 1);
    } while (strlen(buffer) != PESEL_LENGTH);
    strcpy(new->pesel_number, buffer);
}
void getName(account_t *new)
{
    system("clear");
    printf("Enter first name: ");
    getString(new->first_name, CHARBUFFER);
    system("clear");
    printf("Enter surname: ");
    getString(new->last_name, CHARBUFFER);
}
void getLocation(account_t *new)
{
    system("clear");
    printf("Enter address: ");
    getString(new->address, ADDRBUFFER);
}
void getBalance(account_t *new)
{
    system("clear");
    new->balance = getDouble(CASH_MIN, CASH_MAX, "current balance");
}
void getLoanInfo(account_t *new)
{
    system("clear");
    new->bank_loan = getDouble(CASH_MIN, CASH_MAX, "current loan");
    new->interest = (1 + BANK_INTEREST) * new->bank_loan / MONTHS_OF_PAYMENT;
}
void generateIBAN(account_t *new)
{
    IBAN to_be_generated;
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
    } while (isIBANoverlapping(to_be_generated));
    strcpy(new->account_number, to_be_generated);
}
bool isIBANoverlapping(IBAN check_val)
{
    FILE *check_f = fopen(DATA_FILE, "rb");
    if (check_f == NULL)
    {
        printf("Error opening file!\n");
        return false;
    }
    account_t check;
    while (fread(&check, sizeof(account_t), 1, check_f))
    {
        if (strcmp(check.account_number, check_val) == 0)
        {
            fclose(check_f);
            return true;
        }
    }
    fclose(check_f);
    return false;
}
uint32_t getLastID()
{
    FILE *seek_file = fopen(DATA_FILE, "rb");
    if (seek_file == NULL)
    {
        printf("Error opening file!\n");
        return 0;
    }
    account_t last;
    fseek(seek_file, -sizeof(account_t), SEEK_END);
    fread(&last, sizeof(account_t), 1, seek_file);
    fclose(seek_file);
    return last.id;
}
void createAccount()
{
    account_t new;
    new.id = getLastID() + 1;
    strcpy(new.account_number, "PL611090101400000071211287");
    generateIBAN(&new);
    getPESEL(&new);
    getName(&new);
    getLocation(&new);
    getBalance(&new);
    getLoanInfo(&new);
    FILE *append_file = fopen(DATA_FILE, "ab");
    if (append_file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fwrite(&new, sizeof(account_t), 1, append_file);
    fclose(append_file);
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
        break;
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
void chooseDisplayOperation()
{
    void (*functionPointer)(void);
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
    (*functionPointer)();
}
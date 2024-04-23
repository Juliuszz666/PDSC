#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prints.h"
#include "typedefs.h"

void printActions()
{
    system("clear");
    printf("***\t\t\tChoose want you want to do\t\t\t***\n");
    printf("***\t\t\t1.\tModify something\t\t\t***\n");
    printf("***\t\t\t2.\tSee something\t\t\t\t***\n");
    printf("***\t\t\t3.\tHelp\t\t\t\t\t***\n");
    printf("***\t\t\t4.\tQuit program\t\t\t\t***\n");
}
void printModifyingOptions()
{
    system("clear");
    printf("***\t\t\tChoose want you want to do\t\t\t***\n");
    printf("***\t\t\t1.\tCreate a new account\t\t\t***\n");
    printf("***\t\t\t2.\tMake a deposit\t\t\t\t***\n");
    printf("***\t\t\t3.\tMake a withdrawal\t\t\t***\n");
    printf("***\t\t\t4.\tMake a money transfer\t\t\t***\n");
    printf("***\t\t\t5.\tTake a loan\t\t\t\t***\n");
    printf("***\t\t\t6.\tPay a debt\t\t\t\t***\n");
}
void printDisplayOptions()
{
    system("clear");
    printf("***\t\t\tChoose want you want to do\t\t\t***\n");
    printf("***\t\t\t1.\tList all accounts\t\t\t***\n");
    printf("***\t\t\t2.\tSearch an account\t\t\t***\n");
}
void printERANGE()
{
    system("clear");
    perror("Value overflow, operation terminated");
    waitingForQuit();
}
void printSuccess()
{
    system("clear");
    printf("Operation sucessful\n");
    waitingForQuit();
}
void waitingForQuit()
{
    char quit;
    printf("Press q/Q to continue\n");
    while ((quit = getchar()) != 'q' && quit != 'Q')
        ;
    while (getchar() != '\n')
        ;
}
void printSearchOptions()
{
    system("clear");
    printf("Enter by what you want to search\n");
    printf("acc num\t-\taccount number\n");
    printf("fname\t-\tfirst name\n");
    printf("lname\t-\tlast name\n");
    printf("addr\t-\taddress\n");
    printf("pesel\t-\tPESEL number\n");
}
void printHelpMenu()
{
    system("clear");
    printf("This is a simple bank program\n");
    printf("You can create accounts, make deposits, withdrawals, \nmoney transfers, take loans and "
           "pay debts\n");
    printf("You can also list all accounts and search for specific ones\n");
    printf("You choose actions by pressing number assigned to them and then pressing enter\n");
    printf("Press q/Q to continue after every interaction\n");
    waitingForQuit();
}
void getString(char *str, int bufsiz)
{
    fgets(str, bufsiz, stdin);
    if (strlen(str) >= bufsiz - 1)
    {
        while (getchar() != '\n' && getchar() != EOF)
            ;
    }
    str[strcspn(str, "\n")] = '\0';
}
double getDouble(double min, double max, const char *msg)
{
    double value;
    char buffer[CHARBUFFER];
    do
    {
        system("clear");
        printf("Enter value of %s: ", msg);
        getString(buffer, CHARBUFFER);
        value = strtod(buffer, NULL);
        if (value <= min || value > max)
        {
            printf("Value out of range! Try again: ");
        }
    } while (value <= min || value > max);
    return value;
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
void printLine()
{
    for (int i = 0; i < LINE_LENGTH; i++)
    {
        printf("-");
    }
    printf("\n");
}
void printAllList()
{
    printAccounts(NULL, NULL);
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
#include "main.h"

void chooseAction();
void chooseModifyingOperation();
void chooseDisplayOperation();

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
        return;
    }
    (*functionPointer)();
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
        return;
    }
    (*functionPointer)();
}
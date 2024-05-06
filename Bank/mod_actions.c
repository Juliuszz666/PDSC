#include "mod_actions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_actions.h"
#include "prompts.h"
#include "typedefs.h"

void transferMoney()
{
    bool source_found = false;
    bool destination_found = false;
    Account_t source = findAccount("source ", &source_found);
    Account_t destination = findAccount("destination ", &destination_found);
    if (!source_found || !destination_found)
    {
        printf("One of the accounts was not found\n");
        waitingForQuit();
        return;
    }
    double transfer = getDouble(CASH_MIN, CASH_MAX, "transfer");
    if (0 > (source.balance - transfer) || transfer >= CASH_MAX || transfer <= 0 ||
        (destination.balance + transfer) >= CASH_MAX)
    {
        printOutOfRange();
        return;
    }
    else
    {
        source.balance -= transfer;
        destination.balance += transfer;
    }
    Account_t accs[] = {source, destination};
    perfromTransferUpdate(accs);
}
void makeDeposit()
{
    bool found = false;
    Account_t deposit_acc = findAccount("deposit", &found);
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
        printOutOfRange();
        return;
    }
    else
    {
        deposit_acc.balance += deposit_amount;
    }
    performOtherUpdate(deposit_acc);

}
void makeWithdrawal()
{
    bool found = false;
    Account_t withdrawal_acc = findAccount("withdrawal", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    double withdrawal_amount = getDouble(CASH_MIN, CASH_MAX, "withdraw");
    if (withdrawal_amount >= CASH_MAX || withdrawal_amount < 0 ||
        0 > (withdrawal_acc.balance - withdrawal_amount))
    {
        printOutOfRange();
        return;
    }
    else
    {
        withdrawal_acc.balance -= withdrawal_amount;
    }
    performOtherUpdate(withdrawal_acc);
}
void takeLoan()
{
    bool found = false;
    Account_t loan_acc = findAccount("loan", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    double loan = getDouble(CASH_MIN, CASH_MAX, "loan");
    if ((loan_acc.balance + loan) >= CASH_MAX || loan >= LOAN_MAX || loan <= 0)
    {
        printOutOfRange();
        return;
    }
    else
    {
        loan_acc.balance += loan;
        loan_acc.interest = (1 + BANK_INTEREST) * (loan_acc.bank_loan + loan) / MONTHS_OF_PAYMENT;
        loan_acc.bank_loan += loan_acc.interest * MONTHS_OF_PAYMENT;
    }
    performOtherUpdate(loan_acc);
}
void payDebt()
{
    bool found = false;
    Account_t debt_acc = findAccount("debt", &found);
    if (!found)
    {
        printf("Account was not found\n");
        waitingForQuit();
        return;
    }
    if (0 > (debt_acc.balance - debt_acc.interest) || debt_acc.bank_loan == 0)
    {
        printOutOfRange();
    }
    else
    {
        debt_acc.balance -=
            (debt_acc.bank_loan >= debt_acc.interest) ? debt_acc.interest : debt_acc.bank_loan;
        debt_acc.bank_loan -=
            (debt_acc.bank_loan >= debt_acc.interest) ? debt_acc.interest : debt_acc.bank_loan;
    }
    if (debt_acc.bank_loan == 0)
    {
        debt_acc.interest = 0;
    }
    performOtherUpdate(debt_acc);
}

#include "file_actions.h"
#include "prompts.h"
#include <stdio.h>
#include <stdlib.h>

void perfromTransferUpdate(Account_t accs[])
{
    if (confirmation(accs, true))
    {
        printSuccess();
        updateTransfer(accs[0], accs[1]);
    }
    else
    {
        printAbort();
    }
}
void performOtherUpdate(Account_t acc)
{
    if (confirmation(&acc, false))
    {
        printSuccess();
        updateAccount(acc);
    }
    else
    {
        printAbort();
    }
}
bool confirmation(Account_t accounts[], bool is_transfer)
{
    system("clear");
    printListHeader();
    printLine();
    for (int i = 0; i < (is_transfer ? 2 : 1); i++)
    {
        printAccount(accounts[i]);
    }
    printf("Do you want to performs this action?\nIf yes pres Y/y -- otherwise anything else\n");
    int action = getchar();
    while (getchar() != '\n')
        ;
    return (action == 'y' || action == 'Y');
}
void updateAccount(Account_t updated)
{
    FILE *update_f = fopen(DATA_FILE, "rb+");
    if (update_f == NULL)
    {
        perror("Error opening file!\n");
        return;
    }
    if (fseek(update_f, sizeof(Account_t) * (updated.id - 1), SEEK_SET) != 0)
    {
        perror("Error seeking file!\n");
        fclose(update_f);
        return;
    }
    fwrite(&updated, sizeof(Account_t), 1, update_f);
    fclose(update_f);
}
void updateTransfer(Account_t source, Account_t destination)
{
    updateAccount(source);
    updateAccount(destination);
}
Account_t findAccount(const char *msg, bool *found)
{
    Account_t retval;
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
    while (fread(&retval, sizeof(Account_t), 1, search_f))
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
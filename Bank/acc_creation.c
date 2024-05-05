#include "acc_creation.h"
#include "prompts.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void getPESEL(Account_t *new)
{
    PESEL buffer;
    do
    {
        getString(buffer, PESEL_LENGTH + 1, "Enter your PESEL: ", true);
    } while (strlen(buffer) != PESEL_LENGTH || !checkDigits(buffer));
    strcpy(new->pesel_number, buffer);
}
void getName(Account_t *new)
{
    getString(new->first_name, CHARBUFFER, "Enter first name: ", true);
    getString(new->last_name, CHARBUFFER , "Enter surname: ", true);
}
void getLocation(Account_t *new)
{
    getString(new->address, ADDRBUFFER, "Enter address: ", true);
}
void getBalance(Account_t *new)
{
    new->balance = getDouble(CASH_MIN, CASH_MAX, "current balance");
}
void getLoanInfo(Account_t *new)
{
    new->bank_loan = getDouble(CASH_MIN, CASH_MAX, "current loan");
    new->interest = (1 + BANK_INTEREST) * new->bank_loan / MONTHS_OF_PAYMENT;
}
void generateIBAN(Account_t *new)
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
    Account_t check;
    while (fread(&check, sizeof(Account_t), 1, check_f))
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
    Account_t last;
    if(fseek(seek_file, -sizeof(Account_t), SEEK_END) != 0) return 0;
    fread(&last, sizeof(Account_t), 1, seek_file);
    fclose(seek_file);
    return last.id;
}
void createAccount()
{
    Account_t new;
    new.id = getLastID() + 1;
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
    fwrite(&new, sizeof(Account_t), 1, append_file);
    fclose(append_file);
}
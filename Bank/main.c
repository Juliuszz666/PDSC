#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define COUNTRY "PL69"
#define BANK_CODE "211569420"

void chooseAction();
void chooseModifyingOperation();
void chooseDisplayOperation();
void generateIBAN(IBAN, node *);
bool isIBANoverlapping(node *, IBAN);

int main(int argc, char const *argv[])
{
    node *head = NULL;
    while (1)
    {
        chooseAction();
        break;
    }

    return 0;
}

void chooseAction()
{
    printf("***\t\t\tChoose want you want to do\t\t\t***\n");
    printf("***\t\t\t1.\tModify something\t\t\t***\n");
    printf("***\t\t\t2.\tSee something\t\t\t\t***\n");
    int key = getchar();
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
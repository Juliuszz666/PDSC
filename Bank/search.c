#include "search.h"
#include "prompts.h"
#include <stdlib.h>
#include <string.h>

bool findName(Account_t ref, Fixed_string key)
{
    return strstr(ref.first_name, key) != NULL;
}
bool findSurname(Account_t ref, Fixed_string key)
{
    return strstr(ref.last_name, key) != NULL;
}
bool findAddress(Account_t ref, Fixed_string key)
{
    return strstr(ref.address, key) != NULL;
}
bool findPESEL(Account_t ref, Fixed_string key)
{
    return strstr(ref.pesel_number, key) != NULL;
}
bool findAccountNumber(Account_t ref, Fixed_string key)
{
    return strstr(ref.account_number, key) != NULL;
}

void getSearchKey(char *search_key, short len)
{
    getString(search_key, len, "Enter search key: ", true);
}
void searchList()
{
    printSearchOptions();
    bool (*searchFun)(Account_t ref, Fixed_string key);
    short len = CHARBUFFER;
    Fixed_string search_type;
    Address search_key;
    getString(search_type, CHARBUFFER, "", false);
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
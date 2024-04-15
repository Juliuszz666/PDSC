#include <stdio.h>
#include <stdlib.h>

#include "prints.h"

void printActions()
{
    system("clear");
    printf("***\t\t\tChoose want you want to do\t\t\t***\n");
    printf("***\t\t\t1.\tModify something\t\t\t***\n");
    printf("***\t\t\t2.\tSee something\t\t\t\t***\n");
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
    printf("Operation sucessful\nPress q/Q to continue\n");
    waitingForQuit();
}
void waitingForQuit()
{
    char quit;
    while ((quit = getchar()) != 'q' && quit != 'Q')
        ;
    
}
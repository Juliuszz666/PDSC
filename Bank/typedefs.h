#ifndef __DEFS_H__
#define __DEFS_H__

#define IBAN_LENGTH 26
#define PESEL_LENGTH 11

#define CHARBUFFER 50

typedef char IBAN[IBAN_LENGTH + 1];
typedef char PESEL[PESEL_LENGTH + 1];
typedef char fixed_string[CHARBUFFER];

typedef struct
{
    fixed_string first_name;
    fixed_string surname;
} name;
typedef struct
{
    fixed_string country;
    fixed_string city;
    fixed_string street;
    fixed_string st_number;
    fixed_string apart_num;
} location;

#endif //__DEFS_H__
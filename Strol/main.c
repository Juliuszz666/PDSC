#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handleBase(const char *str, int i)
{
    if (((strncmp(str + i, "0x", 2) == 0) || (strncmp(str + i, "0X", 2) == 0)) &&
        isxdigit(str[i + 2]))
    {
        return 16;
    }
    if (str[i] == '0' && str[i+1]<'8'&&str[i+1]>='0')
    {
        return 8;
    }
    if (isdigit(str[i]))
    {
        return 10;
    }
    return -1;
}

int isInRange(int base, char c)
{
    int comp = (int)INFINITY;
    comp = isdigit(c) ? c - '0' : comp;
    if (isalpha(c))
    {
        comp = islower(c) ? c - 'a' + 10 : c - 'A' + 10;
    }
    return comp < base ? comp : -1;
}

long int own_strtol(const char *str, char **ednptr, int base)
{
    errno = 0;
    int i = 0;
    while (isspace(str[i]))
    {
        i++;
    }
    bool is_negative = (str[i] == '-');
    if (is_negative || str[i] == '+')
    {
        i++;
    }

    if (base == 0)
    {
        base = handleBase(str, i);
        if (base == 16)
        {
            i += 2;
        }
        if (base == 8)
        {
            i++;
        }
    }
    if (base == 1 || base < 0 || base > 36)
    {
        if (ednptr != NULL)
        {
            *ednptr = (char *)(str + i);
            printf("%s\n", *ednptr);
        }
        errno = EINVAL;
        return 0;
    }
    if ((base == 16) && ((strncmp(str + i, "0x", 2) == 0) || (strncmp(str + i, "0X", 2) == 0)))
    {
        i += 2;
    }
    long int retval = 0;
    int num = 0;
    bool is_num = false;
    while ((num = isInRange(base, str[i])) != -1 && str[i] != '\0')
    {
        is_num = true;
        if ((retval > (LONG_MAX - num) / base) || (retval < (LONG_MIN + num) / base))
        {
            errno = ERANGE;
        }
        retval *= base;
        retval += is_negative ? -num : num;
        i++;
    }
    if (ednptr != NULL && is_num)
    {
        *ednptr = (char *)(str + i);
    }
    switch (errno)
    {
    case ERANGE:
        return is_negative ? LONG_MIN : LONG_MAX;
        break;

    default:
        break;
    }

    return retval;
}

int main(int argc, char const *argv[])
{
    char *ptr;
    for (size_t i = 1; i < argc; i++)
    {
        long x = own_strtol(argv[i], &ptr, 0);
        printf("%ld\n", x);
        printf("\t%ld\n", strtol(argv[i], NULL, 0));
    }

    return 0;
}

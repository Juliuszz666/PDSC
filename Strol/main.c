#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int handleBase(const char *str)
{
    if ((strncmp(str, "0x", 2) == 0) || (strncmp(str, "0X", 2) == 0))
    {
        return 16;
    }
    if ((strncmp(str, "0b", 2) == 0) || (strncmp(str, "0B", 2) == 0))
    {
        return 2;
    }
    if (isdigit(str[0]))
    {
        return 10;
    }
    return -1;
}

int isInRange(int base, char c)
{
    int comp = (int)INFINITY;
    if (c >= '0' && c <= '9')
    {
        comp = (int)c - '0';
    }
    if (c >= 'A' && c <= 'Z')
    {
        comp = (int)c - 'A' + 10;
    }
    if (c >= 'a' && c <= 'z')
    {
        comp = (int)c - 'a' + 10;
    }
    return comp < base ? comp : -1;
}

long int own_strtol(const char *str, char **ednptr, int base)
{
    errno = 0;
    int i = 0;
    bool is_negative = (str[0] == '-');
    if (is_negative)
    {
        i++;
    }
    if (base == 0)
    {
        base = handleBase(str);
        if (base == 2 || base == 16)
        {
            i += 2;
        }
    }
    if (base == 1 || base < 0 || base > 36)
    {
        return 0;
    }
    long int retval = 0;
    int num = 0;
    while ((num = isInRange(base, str[i])) != -1 && str[i] != '\0')
    {
        if (retval > (LONG_MAX - num) / base)
        {
            errno = ERANGE;
        }
        retval *= base;
        retval += is_negative ? -num : num;
        i++;
    }
    if (ednptr != NULL)
    {
        *ednptr = (char *)(str + i);
    }
    switch (errno)
    {
    case ERANGE:
        fprintf(stderr, errno);
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
        long x = own_strtol(argv[i], &ptr, 10);
        printf("<--------------------->\n");
        printf("%s\t", ptr);
        printf("%ld\n", x);
        // x = own_strtol(argv[i], &ptr, 0);
        // printf("%s\t", ptr);
        // printf("%ld\n", x);
    }

    return 0;
}

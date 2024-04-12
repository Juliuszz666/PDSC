#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int skipWhitespace(const char *str);
bool checkSign(const char c, int *index);
void handleBase(const char *str, int *base, int *i);
int handleZeroBase(const char *str, int i);
void handleHexadec(const char *str, int *i);
int getValue(int base, char c);

int handleZeroBase(const char *str, int i)
{
    if (((strncmp(str + i, "0x", 2) == 0) || (strncmp(str + i, "0X", 2) == 0)))
    {
        return 16;
    }
    if (str[i] == '0')
    {
        return 8;
    }
    if (isdigit(str[i]))
    {
        return 10;
    }
    return -1;
}
void handleBase(const char *str, int *base, int *i)
{
    if (*base == 0)
    {
        *base = handleZeroBase(str, *i);
    }
    if (*base == 1 || *base < 0 || *base > 36)
    {
        errno = EINVAL;
    }
    if (*base == 16)
    {
        handleHexadec(str, i);
    }
}
int getValue(int base, char c)
{
    int comp = (int)INFINITY;
    comp = isdigit(c) ? c - '0' : comp;
    if (isalpha(c))
    {
        comp = islower(c) ? c - 'a' + 10 : c - 'A' + 10;
    }
    return comp < base ? comp : -1;
}
int skipWhitespace(const char *str)
{
    int i = 0;
    while (isspace(str[i]))
    {
        i++;
    }
    return i;
}
bool checkSign(const char c, int *index)
{
    bool is_negative = (c == '-');
    if (is_negative || c == '+')
    {
        (*index)++;
    }
    return is_negative;
}
void handleHexadec(const char *str, int *i)
{
    if ((strncmp(str + *i, "0x", 2) == 0) || (strncmp(str + *i, "0X", 2) == 0))
    {
        *i += 2;
        if (!isxdigit(str[*i]))
        {
            *i -= 2;
        }
    }
}
long int strtol(const char *str, char **ednptr, int base)
{
    int i = skipWhitespace(str);
    bool is_negative = checkSign(str[i], &i);
    handleBase(str, &base, &i);
    if (errno == EINVAL)
    {
        return 0;
    }
    long int retval = 0;
    int num = 0;
    while ((num = getValue(base, str[i])) != -1 && str[i] != '\0')
    {
        if ((retval > (LONG_MAX - num) / base) || (retval < (LONG_MIN + num) / base))
        {
            errno = ERANGE;
        }
        retval *= base;
        retval += is_negative ? -num : num;
        i++;
        if (ednptr != NULL)
        {
            *ednptr = (char *)(str + i);
        }
    }
    if (errno == ERANGE)
    {
        return is_negative ? LONG_MIN : LONG_MAX;
    }
    return retval;
}

#include "bsearch.h"
#include <stdint.h>

int compareStruct(const void *num1, const void *num2)
{
    test_t *left = (test_t *)num1;
    test_t *right = (test_t *)num2;
    return ((left->test_key > right->test_key) - (left->test_key < right->test_key));
}

int compareDouble(const void *num1, const void *num2)
{
    double *left = (double *)num1;
    double *right = (double *)num2;
    return ((*left > *right) - (*left < *right));
}
int compareInt(const void *num1, const void *num2)
{
    int *left = (int *)num1;
    int *right = (int *)num2;
    return ((*left > *right) - (*left < *right));
}
int compareChar(const void *num1, const void *num2)
{
    char *left = (char *)num1;
    char *right = (char *)num2;
    return ((*left > *right) - (*left < *right));
}
void *bsearch(const void *key, const void *base, size_t count, size_t size,
              int (*compare)(const void *, const void *))
{
    __uint8_t *middle = (__uint8_t *)base + (size * (count / 2));
    if (count == 0)
    {
        return NULL;
    }
    if (count == 1)
    {
        return (*compare)(key, middle) ? NULL : middle;
    }
    int status = (*compare)(key, middle);
    switch (status)
    {
    case 0:
        return (void *)middle;
        break;
    case 1:
        return bsearch(key, middle + size, count / 2 - (count % 2 == 0), size, compare);
        break;
    case -1:
        return bsearch(key, base, count / 2, size, compare);
        break;
    }
    return NULL;
}
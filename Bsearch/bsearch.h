#include <stddef.h>

typedef struct {
    int test_key;
    char *test_value;
} test_t;

void *bsearch(const void *key, const void *base, size_t count, size_t size,
              int (*compare)(const void *, const void *));
int compareDouble(const void *num1, const void *num2);
int compareInt(const void *num1, const void *num2);
int compareChar(const void *num1, const void *num2);
int compareStruct(const void *num2, const void *num1);
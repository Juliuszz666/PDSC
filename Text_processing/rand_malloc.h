#pragma once

#include <stddef.h>

#define malloc(x)     rand_malloc(x)
#define calloc(x, y)  rand_calloc(x, y)
#define realloc(x, y) rand_realloc(x, y)

void* rand_malloc(size_t size);
void* rand_calloc(size_t num, size_t size);
void* rand_realloc(void* ptr, size_t size);

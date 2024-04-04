#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/** Rate of random error injection. Acceptable values: 0-100 */
unsigned const ALLOCATION_ERROR_RATE = 20;

bool randomness_initialized = false;

bool check_whether_to_call_real_function()
{
	if (!randomness_initialized) {
		srand(time(NULL));
		randomness_initialized = true;
	}
	int r = rand() % 100;
	return r >= ALLOCATION_ERROR_RATE;
}

void* rand_malloc(size_t size)
{
	if (check_whether_to_call_real_function() || size == 0) {
		return malloc(size);
	}
	return NULL;
}

void* rand_calloc(size_t num, size_t size)
{
	if (check_whether_to_call_real_function() || num == 0 || size == 0) {
		return calloc(num, size);
	}
	return NULL;
}

void* rand_realloc(void* ptr, size_t size)
{
	if (check_whether_to_call_real_function() || size == 0) {
		return realloc(ptr, size);
	}
	return NULL;
}

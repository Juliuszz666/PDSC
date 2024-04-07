#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_basic_cases();
void test_passing_null_end_pointer();

void test_boundary_cases_for_base_16_when_size_of_long_is_4_bytes();
void test_boundary_cases_for_base_16_when_size_of_long_is_8_bytes();

void test_boundary_cases_for_base_10_when_size_of_long_is_4_bytes();
void test_boundary_cases_for_base_10_when_size_of_long_is_8_bytes();

void test_boundary_cases_for_base_36_when_size_of_long_is_4_bytes();
void test_boundary_cases_for_base_36_when_size_of_long_is_8_bytes();

void test_parsing_whitespace_characters();
void test_setting_end_pointer();

void test_decimal_base_recognition();
void test_octal_base_recognition();
void test_hexadecimal_base_recognition();

void test_zero_is_not_misled_with_octal_prefix();
void test_zero_is_not_misled_with_hexadecimal_prefix();

void test_invalid_string();
void test_invalid_base();

void test_strtol(char* s, int base, int printbase);
void print_num(long number, int base);

int main()
{
	test_basic_cases();
	test_passing_null_end_pointer();

	test_boundary_cases_for_base_16_when_size_of_long_is_4_bytes();
	test_boundary_cases_for_base_16_when_size_of_long_is_8_bytes();

	test_boundary_cases_for_base_10_when_size_of_long_is_4_bytes();
	test_boundary_cases_for_base_10_when_size_of_long_is_8_bytes();

	test_boundary_cases_for_base_36_when_size_of_long_is_4_bytes();
	test_boundary_cases_for_base_36_when_size_of_long_is_8_bytes();

	test_parsing_whitespace_characters();
	test_setting_end_pointer();

	test_decimal_base_recognition();
	test_octal_base_recognition();
	test_hexadecimal_base_recognition();

	test_zero_is_not_misled_with_octal_prefix();
	test_zero_is_not_misled_with_hexadecimal_prefix();

	test_invalid_string();
	test_invalid_base();

	return 0;
}

void test_basic_cases()
{
	printf("=== Test basic cases === \n\n");

	test_strtol("", 10, 10);
	test_strtol("0", 16, 16);

	test_strtol("1234", 10, 10);
	test_strtol("-1234", 10, 10);

	test_strtol("F", 16, 16);
	test_strtol("-F", 16, 16);
}

void test_passing_null_end_pointer()
{
	printf("=== Test passing null end pointer === \n\n");

	strtol("1234", NULL, 10);
	strtol("FE", NULL, 16);
}

void test_boundary_cases_for_base_16_when_size_of_long_is_4_bytes()
{
	printf("=== Test boundary cases for base 16 when size of long is 4 bytes "
		   "=== \n\n");

	test_strtol("7FFFFFFE", 16, 16);
	test_strtol("7FFFFFFF", 16, 16);
	test_strtol("80000000", 16, 16);
	test_strtol("80000001", 16, 16);

	test_strtol("-7FFFFFFF", 16, 16);
	test_strtol("-80000000", 16, 16);
	test_strtol("-80000001", 16, 16);
	test_strtol("-80000002", 16, 16);
}

void test_boundary_cases_for_base_16_when_size_of_long_is_8_bytes()
{
	printf("=== Test boundary cases for base 16 when size of long is 8 bytes "
		   "=== \n\n");

	test_strtol("7FFFFFFFFFFFFFFE", 16, 16);
	test_strtol("7FFFFFFFFFFFFFFF", 16, 16);
	test_strtol("8000000000000000", 16, 16);
	test_strtol("8000000000000001", 16, 16);

	test_strtol("-7FFFFFFFFFFFFFFF", 16, 16);
	test_strtol("-8000000000000000", 16, 16);
	test_strtol("-8000000000000001", 16, 16);
	test_strtol("-8000000000000002", 16, 16);
}

void test_boundary_cases_for_base_10_when_size_of_long_is_4_bytes()
{
	printf("=== Test boundary cases for base 10 when size of long is 4 bytes "
		   "=== \n\n");

	test_strtol("2147483646", 10, 10);
	test_strtol("2147483647", 10, 10);
	test_strtol("2147483648", 10, 10);
	test_strtol("2147483649", 10, 10);

	test_strtol("-2147483647", 10, 10);
	test_strtol("-2147483648", 10, 10);
	test_strtol("-2147483649", 10, 10);
	test_strtol("-2147483650", 10, 10);
}

void test_boundary_cases_for_base_10_when_size_of_long_is_8_bytes()
{
	printf("=== Test boundary cases for base 10 when size of long is 8 bytes "
		   "=== \n\n");

	test_strtol("9223372036854775806", 10, 10);
	test_strtol("9223372036854775807", 10, 10);
	test_strtol("9223372036854775808", 10, 10);
	test_strtol("9223372036854775809", 10, 10);

	test_strtol("-9223372036854775807", 10, 10);
	test_strtol("-9223372036854775808", 10, 10);
	test_strtol("-9223372036854775809", 10, 10);
	test_strtol("-9223372036854775810", 10, 10);
}

void test_boundary_cases_for_base_36_when_size_of_long_is_4_bytes()
{
	printf("=== Test boundary cases for base 36 when size of long is 4 bytes "
		   "=== \n\n");

	test_strtol("ZIK0ZI", 36, 36);
	test_strtol("ZIK0ZJ", 36, 36);
	test_strtol("ZIK0ZK", 36, 36);
	test_strtol("ZIK0ZL", 36, 36);

	test_strtol("-ZIK0ZJ", 36, 36);
	test_strtol("-ZIK0ZK", 36, 36);
	test_strtol("-ZIK0ZL", 36, 36);
	test_strtol("-ZIK0ZM", 36, 36);
}

void test_boundary_cases_for_base_36_when_size_of_long_is_8_bytes()
{
	printf("=== Test boundary cases for base 36 when size of long is 8 bytes "
		   "=== \n\n");

	test_strtol("1Y2P0IJ32E8E6", 36, 36);
	test_strtol("1Y2P0IJ32E8E7", 36, 36);
	test_strtol("1Y2P0IJ32E8E8", 36, 36);
	test_strtol("1Y2P0IJ32E8E9", 36, 36);

	test_strtol("-1Y2P0IJ32E8E7", 36, 36);
	test_strtol("-1Y2P0IJ32E8E8", 36, 36);
	test_strtol("-1Y2P0IJ32E8E9", 36, 36);
	test_strtol("-1Y2P0IJ32E8EA", 36, 36);
}

void test_parsing_whitespace_characters()
{
	printf("=== Test parsing whitespace characters === \n\n");

	test_strtol(" \t\n -0x234", 16, 16);
	test_strtol(" \t\n +128", 10, 10);
	test_strtol(" \t\n + 128", 10, 10);
}

void test_setting_end_pointer()
{
	printf("=== Test setting end pointer === \n\n");

	test_strtol("-0x7FFFFFFEEEEEEEEEEE???", 16, 16);
	test_strtol("-0xZZZZZZZZZZZZZZZZ???", 36, 36);

	test_strtol("438488348HU12325235235", 10, 10);
}

void test_decimal_base_recognition()
{
	printf("=== Test decimal base recognition === \n\n");

	test_strtol("9", 0, 10);
	test_strtol("-9", 0, 10);

	test_strtol("21474836481", 0, 10);
	test_strtol("-21474836481", 0, 10);
}

void test_octal_base_recognition()
{
	printf("=== Test octal base recognition === \n\n");

	test_strtol("07", 0, 8);
	test_strtol("-07", 0, 8);

	test_strtol("071", 0, 8);
	test_strtol("-071", 0, 8);
}

void test_hexadecimal_base_recognition()
{
	printf("=== Test hexadecimal base recognition === \n\n");

	test_strtol("0xF", 0, 16);
	test_strtol("-0xF", 0, 16);

	test_strtol("0x", 0, 16);
	test_strtol("-0x", 0, 16);

	test_strtol("0x7FFFFFFE", 0, 16);
	test_strtol("-0x7FFFFFFE", 0, 16);
}

void test_zero_is_not_misled_with_octal_prefix()
{
	printf("=== Test zero is not misled with octal prefix === \n\n");

	test_strtol("08", 0, 8);
	test_strtol("08", 8, 8);

	test_strtol("0r", 0, 8);
	test_strtol("0r", 8, 8);

	test_strtol("-08", 0, 8);
	test_strtol("-08", 8, 8);

	test_strtol("-0r", 0, 8);
	test_strtol("-0r", 8, 8);
}

void test_zero_is_not_misled_with_hexadecimal_prefix()
{
	printf("=== Test zero is not misled with hexadecimal prefix === \n\n");

	test_strtol("0xG", 0, 16);
	test_strtol("0xG", 16, 16);

	test_strtol("0xr", 0, 16);
	test_strtol("0xr", 16, 16);

	test_strtol("-0xG", 0, 16);
	test_strtol("-0xG", 16, 16);

	test_strtol("-0xr", 0, 16);
	test_strtol("-0xr", 16, 16);
}

void test_invalid_string()
{
	printf("=== Test invalid string === \n\n");

	test_strtol("+#$#$", 36, 36);
	test_strtol("-#$#$", 36, 36);
}

void test_invalid_base()
{
	printf("=== Test invalid base === \n\n");

	test_strtol("-#$#$", 40, 10);
	test_strtol("10", 40, 10);
	test_strtol("10", 1, 10);
	test_strtol("0", 1, 10);
	test_strtol("0", -1, 10);
}

void test_strtol(char* s, int base, int printbase)
{
	char* cp = strdup(s);
	char* endPtr = cp;
	errno = 0;

	printf("%s in base %d is ", s, base);
	print_num(strtol(cp, &endPtr, base), printbase);
	putchar('\n');

	printf("strtol: %s\n", strerror(errno));
	if (endPtr == cp)
		printf("String is not a number\n");
	else
		printf("End pointer points to \"%s\"\n", endPtr);
	putchar('\n');
	free(cp);
}

/* print number using specified base, 2 < base < 36 */
void print_num(long number, int base)
{
	long power_of_base;

	if (number >= 0) {
		power_of_base = 1;
		/* Find the greatest power of 'base' which isn't more
		   than the number */
		while (power_of_base <= number / base)
			power_of_base = power_of_base * base;
		/* Now print out the digits */
		while (power_of_base > 0) {
			int dig = number / power_of_base;
			if (dig <= 9)
				dig += '0';
			else
				dig += 'A' - 10;
			putchar(dig);
			number = number % power_of_base;
			power_of_base = power_of_base / base;
		}
	}
	else /* number <0 */
	{
		putchar('-');
		power_of_base = -1;
		/* cannot just negate the number - need a symmetrical case */
		while (power_of_base >= number / base)
			power_of_base = power_of_base * base;

		while (power_of_base < 0) {
			int dig = number / power_of_base;
			if (dig <= 9)
				dig += '0';
			else
				dig += 'A' - 10;
			putchar(dig);
			/* % for negative numbers is not well defined in C */
			number = number - (number / power_of_base) * power_of_base;
			power_of_base = power_of_base / base;
		}
	}
}

/* =============== GPL HEADER =====================
 * TuxStrings.c is part of TuxDroidServer
 * Copyleft (C) 2012 - Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 * ====================================================
*/

#include <stdlib.h>
#include <string.h>
#include <unity.h>

#include "TuxStrings.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void testExplodeShouldReturnNULLWhenGivenNULL(void)
{
	TEST_ASSERT_EQUAL(NULL, explode(NULL, 'z'));
}

void testExplodeShouldReturnEmptyStringWhenStringIsEmpty(void)
{
	char *empty_string = "";
	char *expected = "";

	char **result = explode(empty_string, 'z');

	TEST_ASSERT_EQUAL_STRING(expected, result[0]);
	TEST_ASSERT_EQUAL(NULL, result[1]);

	freeExplode(result);
}

void testExplodeShouldReturnSameStringWhenSeparatorIsNotFound(void)
{
	char test_string[] = "abcd";
	char expected[] = "abcd";

	char **result = explode(test_string, 'z');

	TEST_ASSERT_EQUAL_STRING(expected, result[0]);
	TEST_ASSERT_EQUAL(NULL, result[1]);

	freeExplode(result);
}

void testExplodeShouldReturnTwoEmptyStringsWhenOnlySeparatorIsFound(void)
{
	char test_string[] = "z";
	char expected_0[] = "";
	char expected_1[] = "";

	char **result = explode(test_string, 'z');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL(NULL, result[2]);

	freeExplode(result);
}

void testExplodeShouldReturnAllButFirstWhenSeparatorIsFirst(void)
{
	char test_string[] = "abcd";
	char expected_0[] = "";
	char expected_1[] = "bcd";

	char **result = explode(test_string, 'a');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL(NULL, result[2]);

	freeExplode(result);}


void testExplodeShouldReturnAllButLastWhenSeparatorIsLast(void)
{
	char test_string[] = "abcd";
	char expected_0[] = "abc";
	char expected_1[] = "";

	char **result = explode(test_string, 'd');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL(NULL, result[2]);

	freeExplode(result);
}

void testExplodeShouldReturnTwoSubStringsWhenSeparatorIsInMiddle(void)
{
	char test_string[] = "abcde";
	char expected_0[] = "ab";
	char expected_1[] = "de";

	char **result = explode(test_string, 'c');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL(NULL, result[2]);

	freeExplode(result);
}

void testExplodeShouldReturnThreeEmptySubStringsWhenOnlyTwoSeparatorsAreFound(void)
{
	char test_string[] = "bb";
	char expected_0[] = "";
	char expected_1[] = "";
	char expected_2[] = "";

	char **result = explode(test_string, 'b');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL_STRING(expected_2, result[2]);
	TEST_ASSERT_EQUAL(NULL, result[3]);

	freeExplode(result);
}

void testExplodeShouldReturnThreeSubStringsWhenTwoSeparatorsAreInMiddle(void)
{
	char test_string[] = "abebi";
	char expected_0[] = "a";
	char expected_1[] = "e";
	char expected_2[] = "i";

	char **result = explode(test_string, 'b');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL_STRING(expected_2, result[2]);
	TEST_ASSERT_EQUAL(NULL, result[3]);

	freeExplode(result);
}

void testExplodeShouldReturnOneEmptySubStringsWhenTwoSeparatorsAreContiguous(void)
{
	char test_string[] = "abbc";
	char expected_0[] = "a";
	char expected_1[] = "";
	char expected_2[] = "c";

	char **result = explode(test_string, 'b');

	TEST_ASSERT_EQUAL_STRING(expected_0, result[0]);
	TEST_ASSERT_EQUAL_STRING(expected_1, result[1]);
	TEST_ASSERT_EQUAL_STRING(expected_2, result[2]);
	TEST_ASSERT_EQUAL(NULL, result[3]);

	freeExplode(result);
}

void testCountCharacterOccurency(void)
{
	TEST_ASSERT_EQUAL(0, countCharacterOccurency("", 'a'));
	TEST_ASSERT_EQUAL(0, countCharacterOccurency("abc", 'z'));
	TEST_ASSERT_EQUAL(1, countCharacterOccurency("abcdef", 'd'));
	TEST_ASSERT_EQUAL(2, countCharacterOccurency("abebi", 'b'));
	TEST_ASSERT_EQUAL(3, countCharacterOccurency("zzz", 'z'));
}

#define STR_TO_UPPER_TEST_STR_LEN 64
void assert_equal_uppercased(char *expected_string, char *test_string)
{
	char writable_test_string[STR_TO_UPPER_TEST_STR_LEN];
	strncpy(writable_test_string, test_string, STR_TO_UPPER_TEST_STR_LEN);

	TEST_ASSERT_EQUAL_STRING(expected_string, strtoupper(writable_test_string));
}

void testToUpperShouldReturnNULLWhenStringIsNULL(void)
{
	TEST_ASSERT_EQUAL(NULL, strtoupper(NULL));
}

void testToUpperShouldReturnAllLettersUppercase(void)
{
	assert_equal_uppercased("", "");
	assert_equal_uppercased("123", "123");
	assert_equal_uppercased(" -&=+]", " -&=+]");
	assert_equal_uppercased("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz");
	assert_equal_uppercased("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	assert_equal_uppercased("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ABCDEFGHIJKLMNOPQRSTuvWxYz");
}

#define STR_TO_LOWER_TEST_STR_LEN 64
void assert_equal_lowercased(char *expected_string, char *test_string)
{
	char writable_test_string[STR_TO_LOWER_TEST_STR_LEN];
	strncpy(writable_test_string, test_string, STR_TO_LOWER_TEST_STR_LEN);

	TEST_ASSERT_EQUAL_STRING(expected_string, strtolower(writable_test_string));
}

void testToLowerShouldReturnNULLWhenStringIsNULL(void)
{
	TEST_ASSERT_EQUAL(NULL, strtolower(NULL));
}

void testToLowerShouldReturnAllLettersLowercase(void)
{
	assert_equal_lowercased("", "");
	assert_equal_lowercased("123", "123");
	assert_equal_lowercased(" -&=+]", " -&=+]");
	assert_equal_lowercased("abcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyz");
	assert_equal_lowercased("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	assert_equal_lowercased("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTuvWxYz");
}

void testTrimShouldReturnNULLWhenStringIsNULL(void)
{
	TEST_ASSERT_EQUAL(NULL, trim(NULL));
}

#define TRIM_TEST_STR_LEN 64
void assert_equal_trimmed(char *expected_string, char *test_string)
{
	char writable_test_string[TRIM_TEST_STR_LEN];
	strncpy(writable_test_string, test_string, TRIM_TEST_STR_LEN);

	TEST_ASSERT_EQUAL_STRING(expected_string, trim(writable_test_string));
}

void testTrimShouldReturnEmptyStringWhenStringIsEmptyOrOnlySpacesOrNewlines(void)
{
	assert_equal_trimmed("", "");
	assert_equal_trimmed("", " ");
	assert_equal_trimmed("", "\n");
	assert_equal_trimmed("", "   ");
	assert_equal_trimmed("", "\n\n\n");
	assert_equal_trimmed("", "\n \n  \n");
}

void testTrimShouldReturnSameStringWhenStringHasNoLeadingOrTrailingSpacesOrNewlines(void)
{
	assert_equal_trimmed("abc", "abc");
	assert_equal_trimmed("a\nb \n \n c", "a\nb \n \n c");
	assert_equal_trimmed("a\nb\n\nc", "a\nb\n\nc");
	assert_equal_trimmed("a b  c", "a b  c");
}

void testTrimShouldRemoveLeadingSpacesAndNewlines(void)
{
	assert_equal_trimmed("abc", " abc");
	assert_equal_trimmed("abc", "   abc");
	assert_equal_trimmed("abc", "\nabc");
	assert_equal_trimmed("abc", "\n\nabc");
	assert_equal_trimmed("abc", "\n \n \nabc");
}

void testTrimShouldRemoveTrailingSpacesAndNewlines(void)
{
	assert_equal_trimmed("abc", "abc ");
	assert_equal_trimmed("abc", "abc   ");
	assert_equal_trimmed("abc", "abc\n");
	assert_equal_trimmed("abc", "abc\n\n\n");
	assert_equal_trimmed("abc", "abc\n \n \n");
}

void testTrimShouldRemoveLeadingAndTrailingSpacesAndNewlines(void)
{
	assert_equal_trimmed("abc", "\n \n \nabc\n \n \n");
}

void testDuplicateStringShouldReturnNULLWhenCalledWithNULL(void)
{
	TEST_ASSERT_EQUAL(NULL, duplicate_string(NULL));
}

void testDuplicateStringShouldReturnAnotherStringWithTheSameContent(void)
{
	char test_string[] = "test";

	char *result = duplicate_string(test_string);

	TEST_ASSERT_EQUAL_STRING(test_string, result);
	TEST_ASSERT_NOT_EQUAL(test_string, result);
}

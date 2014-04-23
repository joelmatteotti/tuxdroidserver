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
#include <unistd.h>

#include <unity.h>
#include "TuxUtils.h"

#define SHELL_COMMAND_LEN 256
#define PATH_LEN 256
char old_cwd[PATH_LEN];

void _saveCWD();
void _restoreCWD();
void _createTemporaryDirectory(char *directory_path);
void _createTemporaryFile(char *file_path);
void _generateTemporaryPath(char *path);
void _createDirectory(char *directory_path);
void _deleteDirectory(char *directory_path);
void _createFile(char *file_path);
void _deleteFile(char *file_path);

void setUp(void)
{
	_saveCWD();
}

void tearDown(void)
{
	_restoreCWD();
}

#ifndef _WIN32 /* TODO: make windows compatible */
void testGetCurrentDir(void)
{
	char current_working_directory[PATH_LEN];

	_createTemporaryDirectory(current_working_directory);
	chdir(current_working_directory);
	char *expected = current_working_directory;

	char *result = getCurrentDir();

	TEST_ASSERT_EQUAL_STRING(expected, result);

	free(result);
	_deleteDirectory(current_working_directory);
}
#endif

#ifndef _WIN32 /* TODO: make windows compatible */
void testFileExistsTrue(void)
{
	char temporary_file[PATH_LEN];

	_createTemporaryFile(temporary_file);

	bool result = file_exists(temporary_file);

	TEST_ASSERT_TRUE(result);

	_deleteFile(temporary_file);
}
#endif

#ifndef _WIN32 /* TODO: make windows compatible */
void testFileExistsFalse(void)
{
	char temporary_file[PATH_LEN];

	_generateTemporaryPath(temporary_file);

	bool result = file_exists(temporary_file);

	TEST_ASSERT_FALSE(result);
}
#endif

#ifndef _WIN32 /* TODO: make windows compatible */
void testDirectoryExistsTrue(void)
{
	char temporary_directory[PATH_LEN];

	_createTemporaryFile(temporary_directory);

	bool result = directory_exists(temporary_directory);

	TEST_ASSERT_TRUE(result);

	_deleteDirectory(temporary_directory);
}
#endif

#ifndef _WIN32 /* TODO: make windows compatible */
void testDirectoryExistsFalse(void)
{
	char temporary_directory[PATH_LEN];

	_generateTemporaryPath(temporary_directory);

	bool result = directory_exists(temporary_directory);

	TEST_ASSERT_FALSE(result);
}
#endif

#define ISO8859_TO_UTF8_TEST_STR_LEN 64
void assert_equal_converted_to_utf8(char *expected_string, char *string)
{
	char result_string[ISO8859_TO_UTF8_TEST_STR_LEN];
	iso8859_to_utf8(string, result_string);
	TEST_ASSERT_EQUAL_STRING(expected_string, result_string);
}

void testIso8859ToUtf8ShouldNotCrashWhenParametersAreNULL(void)
{
	char *test_string = "";
	iso8859_to_utf8(NULL, NULL);
	iso8859_to_utf8(NULL, test_string);
	iso8859_to_utf8(test_string, NULL);
}

void testIso8859ToUtf8(void)
{
	assert_equal_converted_to_utf8("", "");
	assert_equal_converted_to_utf8("abc", "abc");
	assert_equal_converted_to_utf8("\xc3\xa0""bcd""\xc3\xa9""f", "\xe0""bcd""\xe9""f");
}

void _saveCWD()
{
	getcwd(old_cwd, PATH_LEN);
}

void _restoreCWD()
{
	chdir(old_cwd);
}

void _createTemporaryDirectory(char *directory_path)
{
	_generateTemporaryPath(directory_path);
	_createDirectory(directory_path);
}

void _createTemporaryFile(char *file_path)
{
	_generateTemporaryPath(file_path);
	_createFile(file_path);
}

void _generateTemporaryPath(char *path)
{
	tmpnam(path);
}

void _createDirectory(char *directory_path)
{
	char command[SHELL_COMMAND_LEN];

	sprintf(command, "mkdir %s", directory_path);
	system(command);
}

void _deleteDirectory(char *directory_path)
{
	rmdir(directory_path);
}

void _createFile(char *file_path)
{
	char command[SHELL_COMMAND_LEN];

	sprintf(command, "touch %s", file_path);
	system(command);
}

void _deleteFile(char *file_path)
{
	remove(file_path);
}

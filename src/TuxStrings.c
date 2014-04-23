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


#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <TuxStrings.h>
#include <stdbool.h>
#include <ctype.h>


char *replace(const char *strbuf, const char *strold, const char *strnew)
{
	char *strret, *p = NULL;
	char *posnews, *posold;
	size_t szold = strlen(strold);
	size_t sznew = strlen(strnew);
	size_t n = 1;

	if(!strbuf)
		return NULL;

	if(!strold || !strnew || !(p = strstr(strbuf, strold)))
		return duplicate_string(strbuf);

	while(n > 0)
	{
		if(!(p = strstr(p+1, strold)))
			break;
		n++;
	}

	strret = (char*)malloc(strlen(strbuf)-(n*szold)+(n*sznew)+1);

	p = strstr(strbuf, strold);

	strncpy(strret, strbuf, (p-strbuf));
	strret[p-strbuf] = 0;
	posold = p+szold;
	posnews = strret+(p-strbuf);
	strcpy(posnews, strnew);
	posnews += sznew;

	while(n > 0)
	{
		if(!(p = strstr(posold, strold)))
			break;

		strncpy(posnews, posold, p-posold);
		posnews[p-posold] = 0;
		posnews += (p-posold);
		strcpy(posnews, strnew);
		posnews += sznew;
		posold = p+szold;
	}

	strcpy(posnews, posold);

	return strret;
}


/*
 Explode a string by a specific character and put results in an char* array, ending by NULL.
*/
char **explode(char *str, char separator)
{
	if (str == NULL) {
		return NULL;
	}

	char *new_chunk;
	int chunk_start = 0;
	int len = strlen(str), chunk_len = 0;
	char **res = (char **) malloc(sizeof (char *));
	int res_len = 1;
	int i;

	for (i = 0; i <= len; ++i)
	{
		if ((i == len) || (str[i] == separator))
		{
			res = (char **) realloc(res, ++res_len * sizeof (char *));
			chunk_len = i - chunk_start;
			new_chunk = (char *) malloc((chunk_len + 1) * sizeof (char));
			res[res_len - 2] = new_chunk;

			strncpy(new_chunk, str + chunk_start, chunk_len);
			new_chunk[chunk_len] = '\0';
			chunk_start = i + 1;
		}
	}
	res[res_len - 1] =  NULL;
	return res;
}

/* Free memory allocation by explode */
void freeExplode(char **exploded_string)
{
	if (exploded_string == NULL) {
		return;
	}
	char **current_chunk = exploded_string;
	while (*current_chunk != NULL)
	{
		free(*current_chunk);
		*current_chunk = NULL;
		current_chunk ++;
	}
	free(exploded_string);
}

/* Count occurrences of a char in string */
int countCharacterOccurency(char str[], char car)
{
	int count = 0, k = 0;

	while (str[k] != '\0')
	{
		if (str[k] == car)
			count++;
		k++;
	}

	return count;
}

char *apply_to_all_chars(int (function)(int), char *string)
{
	char *current_char = string;
	while (*current_char != '\0')
	{
		*current_char = function(*current_char);
		current_char++;
	}

	return string;
}

/* Convert a string to uppercase */
char *strtoupper(char *string)
{
	if (string == NULL)
		return NULL;

	string = apply_to_all_chars(toupper, string);

	return string;
}

/* Convert a string to lowercase */
char *strtolower(char *string)
{
	if (string == NULL)
		return NULL;

	string = apply_to_all_chars(tolower, string);

	return string;
}

/* Trim spaces in left-side string */
char *trim_left(char *string)
{
	char *string_start;
	for (string_start = string;
	     *string_start && isspace(*string_start);
	     ++string_start);

	// +1 to include the trailing \0
	memmove(string, string_start, strlen(string_start) + 1);
	return string;
}

/* Trim spaces in right-side string */
char *trim_right(char *string)
{
	char *string_end;
	for (string_end = string + strlen(string);
	     string_end > string && isspace(*(string_end - 1));
	     --string_end);

	*string_end = '\0';
	return string;
}

char *trim(char *string)
{
	if (string == NULL)
	{
		return NULL;
	}

	string = trim_left(string);
	string = trim_right(string);
	return string;
}

char *duplicate_string(const char *string)
{
	if (string == NULL) {
		return NULL;
	}
	unsigned bytes_to_allocate = strlen(string) + 1;  /* +1 for the trailing \0 */
	char *new_pointer = (char *) malloc(bytes_to_allocate * sizeof(char));
	strcpy(new_pointer, string);
	return new_pointer;
}

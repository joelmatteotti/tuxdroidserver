/* =============== GPL HEADER =====================
 * TuxUtils.c is part of TuxDroidServer
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

#include <TuxUtils.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <TuxStrings.h>

#ifdef _WIN32
	#include <windows.h> /* CharToOemA() */
#endif

#define PRINT_MAX_LEN 4096


char *getCurrentDir()
{
	unsigned max_path_length = 256;
	char *path=(char *)malloc(sizeof(char) * max_path_length);
	getcwd(path, max_path_length);
	return path;
}


bool file_exists(const char *filename)
{
	bool res=false;
	FILE *fp = fopen(filename,"r");
	if(fp != NULL)
	{
		res=true;
		fclose(fp);
	}

	return res;
}

bool directory_exists(const char *directory)
{
	struct stat st;
	if(stat(directory,&st) == 0)
		return true;

	return false;
}

/* Out buffer MUST be twice long as the original string */
void iso8859_to_utf8(char *string, char *out_buffer)
{
	if (string == NULL || out_buffer == NULL)
	{
		return;
	}

	char *out_cursor = out_buffer;
	while(*string != '\0')
	{
		unsigned char c = (unsigned char) *string;
		if(c < 128)
		{
			*out_cursor = c;
			out_cursor++;
		}
		else
		{
			*out_cursor = (c >> 6) | 0xC0;
			out_cursor++;
			*out_cursor = (c & 0x3F) | 0x80;
			out_cursor++;
		}
		string++;
	}
	*out_cursor = '\0';
}

/* Replace printf() to handle accented characters */
void tprintf(const char *format, ...)
{
	va_list argument_list;
	char printed_text[PRINT_MAX_LEN];

	va_start(argument_list, format);
	vsnprintf(printed_text, PRINT_MAX_LEN, format, argument_list);
	va_end(argument_list);

	char *encoded_string;

	#ifdef _WIN32
	{
		encoded_string = (char *) malloc (sizeof(char) * strlen(printed_text) + 1);
		CharToOemA(printed_text, encoded_string);
	}
	#else
	{
		encoded_string = (char *) malloc (sizeof(char) * strlen(printed_text) * 2 + 1);
		iso8859_to_utf8(printed_text, encoded_string);
	}
	#endif

	printf("%s", encoded_string);

	free(encoded_string);
	encoded_string = NULL;
}

leds_t getLedsFromString(char *led)
{
	leds_t LED;
	if(!strcmp(strtolower(led),"left"))
		LED = LED_LEFT;
	if(!strcmp(strtolower(led),"right"))
		LED = LED_RIGHT;
	if(!strcmp(strtolower(led),"both"))
		LED = LED_BOTH;

	return LED;
}

effect_type_t getEffectFromString(char *effect)
{
	effect_type_t EFFECT = NONE;

	if(!strcmp(strtoupper(effect),"NONE"))
		EFFECT=NONE;
	if(!strcmp(strtoupper(effect),"LAST"))
		EFFECT=LAST;
	if(!strcmp(strtoupper(effect),"DEFAULT"))
		EFFECT=DEFAULT;
	if(!strcmp(strtoupper(effect),"FADE_DURATION"))
		EFFECT=FADE_DURATION;
	if(!strcmp(strtoupper(effect),"FADE_RATE"))
		EFFECT=FADE_RATE;
	if(!strcmp(strtoupper(effect),"GRADIENT_NBR"))
		EFFECT=GRADIENT_NBR;
	if(!strcmp(strtoupper(effect),"GRADIENT_DELTA"))
		EFFECT=GRADIENT_DELTA;

	return EFFECT;
}

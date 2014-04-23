/* =============== GPL HEADER =====================
 * TuxLang.h is part of TuxDroidServer
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

#ifndef __TUXLANG_H__
#define __TUXLANG_H__

#include <stdio.h>
#include <stdbool.h>
#include <TuxStrings.h>
#include <TuxUtils.h>

#define TRANSLATIONS_NUMBER	21		/* Define how many translations exist*/

typedef struct
{
	char *key;	/* Contains the key to get the sentence. Ex: HEADER_L1 */
	char *str;	/* Contains the sentence. Ex: HEADER_L1 => Project's developer: */
} tux_translation_t;
typedef tux_translation_t *tux_translation;

bool TuxLang_loadTranslation(char *_langfile_);
char *TuxLang_getTranslation(char *_key_);

#endif

/* =============== GPL HEADER =====================
 * TuxAttitune.h is part of TuxDroidServer
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

#ifndef __TUXATTITUNE_H__
#define __TUXATTITUNE_H__

#include <stdio.h>
#include <stdbool.h>
#include <libxml/xmlreader.h>
#include <TuxTypes.h>

/* Attitune's block structure */
typedef struct
{
	double start_time;
	int count;
	int speed;
	char *cmd; /* COMMAND */
	char *wav; /* MUSIC */
	double duration;
	char *text; /* TTS */
	int pitch;	/* TTS */
	int track; /* SOUND FLASH */
} attitune_block_t;
typedef attitune_block_t *attitune_block;

/* Attitune's structure */
typedef struct
{
	char *name;
	char *desc;
	char *author;
	char *category;
	char *sub_cat;
	char *language;
	char *keywords;
	char *version;
	char *directory;
	double length;

	attitune_block attitune_block[4096]; /* 4096 blocks maximum per attitune (TODO: dynamic size) */
	int total_blocks;
} attitune_t;
typedef attitune_t *attitune;

/* Prototypes */
int TuxAttitune_unzipAttitune(const char *attfile);
void TuxAttitune_loadAttitune(tux_client client, char *attfile);
void TuxAttitune_processNode(xmlTextReaderPtr reader);
bool TuxAttitune_ParseXMLFile(const char *filename);
void TuxAttitune_writeMacroCMD(attitune_block block);
void *TuxAttitune_startAttituneWav(void *data);
void *TuxAttitune_startAttituneTTS(void *data);


#endif
